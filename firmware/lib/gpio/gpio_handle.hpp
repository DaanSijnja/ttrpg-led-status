#pragma once

#include <Arduino.h>

namespace atmel
{
   class gpio_handle
    {
        public:
            inline auto set_mode(const int mode) -> void {
                pinMode( pin, mode);
            }

            inline auto put(const int value) -> void {
                digitalWrite(pin,value);
            }

            [[nodiscard]] inline auto get() noexcept -> bool {
                return digitalRead(pin);
            }

            [[nodiscard]] inline auto get_pin() noexcept -> uint8_t {
                return pin;
            }

        protected:
            uint8_t pin;

            inline constexpr gpio_handle(
                    const uint8_t pin
                ) noexcept : 
                    pin(pin)
                {}


    }; 
}

