#include <Arduino.h>
#include <gpio.hpp>
#include <sn74hc595.hpp>

const uint8_t SER = 7;
const uint8_t CLK = 9;
const uint8_t LTH = 8;

atmel::gpio ser_pin(7);
atmel::gpio clk_pin(9);
atmel::gpio latch_pin(8);

sn74hc595<2,atmel::gpio *> chip(&ser_pin, &clk_pin, &latch_pin);


void setup() {
    chip.init();
}

void loop() {
    uint8_t output[] = {0b11111111,0b00000011};
    chip.set_output(output,MSBFIRST);
}

