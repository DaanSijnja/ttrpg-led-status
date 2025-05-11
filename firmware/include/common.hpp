#pragma once
#include <Arduino.h>
#include <gpio.hpp>
#include <counter.hpp>
#include <ec11e.hpp>

namespace common {

    static auto frequenty_to_clock_ticks(const unsigned int freq, const unsigned int cpu_speed, const unsigned int prescaler) -> unsigned int {
        return ( (cpu_speed) / (prescaler * freq) ) - 1;
    }

    [[maybe_unused]] static auto ms_to_clock_ticks(const unsigned int ms, const unsigned int cpu_speed, const unsigned int prescaler) -> unsigned int {
        return frequenty_to_clock_ticks((1000 / ms), cpu_speed, prescaler);
    }

    constexpr size_t BAR_SIZE = 10;

    auto clamp_encoder(ec11e<arduino::gpio *> * encoder, int min, int max) -> uint16_t {
        int16_t value = encoder->get_encoder_value();
        if(value < min){
            encoder->set_encoder_value(min);
            value = min;
        }

        if(value > max){
            encoder->set_encoder_value(max);
            value = max;
        }

        return value;
    }

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

    class blink {
        public:
            constexpr blink(counter::clock * c, unsigned int blink_speed) noexcept : 
                compare(counter::compare(c,blink_speed)),
                blinked(false)
            {}

            auto check() -> bool {
                if(compare.compare_count()){
                    blinked = !blinked;
                }
                return blinked;
            }

            auto set_interval(unsigned int interval) -> void {
                compare.set_interval(interval);
            }

            auto set_blinked(bool value) -> void {
                blinked = !value;                            //set it to the oposide of the value set to set it on the next check
                compare.set_last_interval_to_current();
            }

        private:
            counter::compare compare;
            bool blinked;
    };   

    struct status_led_check {
        public:
            size_t index_value;
            color led_color;

            constexpr status_led_check(size_t index_value, color led_color) noexcept :
                index_value(index_value),
                led_color(led_color)
            {}

    };


}
