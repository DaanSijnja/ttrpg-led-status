#include <Arduino.h>

#include <gpio.hpp>
#include <sn74hc595.hpp>
#include <ec11e.hpp>
#include <counter.hpp>

#include "common.hpp"
#include "settings.hpp"

common::led_bar current_led_bar = common::led_bar();

arduino::gpio ser_pin(7);
arduino::gpio clk_pin(9);
arduino::gpio latch_pin(8);

arduino::gpio encoder_pin_a(2);
arduino::gpio encoder_pin_b(3);
arduino::gpio encoder_button(1);

counter::clock counter_clock;
common::blink blink( &counter_clock, settings::COUNTER_FREQUENCY / settings::BLINK_FREQUENCY);

sn74hc595<2,arduino::gpio *> shift_reg(&ser_pin, &clk_pin, &latch_pin);
ec11e<arduino::gpio *> encoder(&encoder_pin_a, &encoder_pin_b, &encoder_button);

const float step_size = static_cast<float>(common::BAR_SIZE) / static_cast<float>(settings::TOTAL_STEPS);
uint16_t prev_encoder_value = -1;

/// @brief Initalizes the interrupts used for the ec11e encoder and the timer used for counter_clock
auto ISR_init() -> void {
    //encoder related
    MCUCR = (1 << ISC00);                   // set mode to change for INT0
    GIMSK = (1 << INT0) | (1 << PCIE1);     // set interupt enable to INT0 and PCIE1
    PCMSK1 = (1 << PCINT9);                 // set only PCINT9 to give interrupts (pin PB1)

    //timer related
    TCCR0A = (1 << WGM01);                 // set to CTC mode 
    TCCR0B = (1 << CS01) | (1 << CS00);
    TIMSK0 = (1 << OCIE0A);

    const auto clock_value = common::frequenty_to_clock_ticks(settings::COUNTER_FREQUENCY, 8000000u, 64u);
    if(clock_value <= 255){
        OCR0A = clock_value;
    } else {
        OCR0A = 255;
    }
}

void setup() {
    ISR_init();
    shift_reg.init();
    encoder.init();
}

void loop() {
    static float bar_value, spare;
    uint16_t encoder_value = common::clamp_encoder(&encoder, 0, settings::TOTAL_STEPS) ;

    if(prev_encoder_value != encoder_value) {
        bar_value = step_size * encoder_value;
        spare = bar_value - floor(bar_value);
        blink.set_interval( static_cast<unsigned int>(static_cast<float>(settings::COUNTER_FREQUENCY / settings::BLINK_FREQUENCY) * spare )  ); 
    }
    
    if(!encoder.get_button_value()) {
        encoder.set_button_value(true);
        encoder.set_encoder_value(0);
        bar_value = 0;
    }

    current_led_bar.fill_bar( static_cast<size_t>( floor(bar_value) ) );

    if(spare > 0.0) {
        

        if(blink.check()) {
            current_led_bar.fill_bar( static_cast<size_t>( floor(bar_value) ) + 1 );
        } 
    }
        
    uint16_t output_bit = current_led_bar.parse();
    uint8_t output[] = {output_bit & 0xFF, (output_bit >> 8) & 0xFF} ;
    shift_reg.set_output(output,MSBFIRST);
}


//encoder related ISR
ISR (PCINT1_vect) {
  encoder.update_button();
}

ISR (INT0_vect) {
  encoder.update_encoder();
}

//timer related ISR
ISR (TIMER0_COMPA_vect) {
    counter_clock.up_count(1);
}