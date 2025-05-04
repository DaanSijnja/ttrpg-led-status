#pragma once
#include <Arduino.h>

namespace common {

    constexpr size_t BAR_SIZE = 10;

    struct color {
        public:
            constexpr color(bool r, bool g, bool b) noexcept :
                red(r),
                green(g),
                blue(b)
            {}

            constexpr color(uint8_t data) noexcept :
                red( ( data & (1<<0) ) ),
                green( ( data & (1<<1) ) ),
                blue( ( data & (1<<2) ) )
            {}

            auto parse() -> uint8_t {
                return  (red << 0) | (green << 1) | (blue << 2);
            }

        private:
            bool red;
            bool green;
            bool blue;

    };

    struct led_bar {
        public:

            constexpr led_bar() :
            rgb_led(0),
            bar(0)
            {}

            constexpr led_bar(uint16_t data) :
            rgb_led(data >> 10),
            bar(data & 0b1111111111)
            {}

            auto set_led(color c) -> void {
                rgb_led = c;
            }

            auto set_bar(size_t index, bool value) -> void {
                bar = (bar & ~(1 << index)) | (value << index);
            }

            auto fill_bar(size_t index) -> void {
                bar = (  0b1111111111 >> (BAR_SIZE - index) ) ;
            }

            auto parse() -> uint16_t {
                return ( ( rgb_led.parse() << 10 ) | (bar & 0b1111111111) );
            }

        private:
            color rgb_led;
            uint16_t bar;

        
    };

   
}
