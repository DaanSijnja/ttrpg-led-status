#include <Arduino.h>

#include <gpio.hpp>
#include <sn74hc595.hpp>
#include <ec11e.hpp>

#include "common.hpp"

common::led_bar current_led_bar = common::led_bar();

atmel::gpio ser_pin(7);
atmel::gpio clk_pin(9);
atmel::gpio latch_pin(8);

atmel::gpio encoder_pin_a(2);
atmel::gpio encoder_pin_b(3);
atmel::gpio encoder_button(1);

sn74hc595<2,atmel::gpio *> shift_reg(&ser_pin, &clk_pin, &latch_pin);

ec11e<atmel::gpio *> encoder(&encoder_pin_a, &encoder_pin_b, &encoder_button);

/// @brief Initalizes the interrupts used for the ec11e encoder
auto ISR_init() -> void {
    MCUCR = (1 << ISC00);                   // set mode to change for INT0
    GIMSK = (1 << INT0) | (1 << PCIE1);     // set interupt enable to INT0 and PCIE1
    PCMSK1 = (1 << PCINT9);                 // set only PCINT9 to give interrupts (pin PB1)
}

void setup() {
    ISR_init();
    shift_reg.init();
    encoder.init();
}

void loop() {

    int16_t encoder_value = encoder.get_encoder_value();

    if(encoder_value < 0){ 
        encoder_value = 0;
        encoder.set_encoder_value(encoder_value);
    }

    if(encoder_value > 10){ 
        encoder_value = 10;
        encoder.set_encoder_value(encoder_value);
    }   

    if(!encoder.get_button_value()) {
        encoder.set_button_value(true);
        encoder.set_encoder_value(0);
        encoder_value = 0;
    }

    current_led_bar.fill_bar(encoder_value);

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