#pragma once

#include "gpio_handle.hpp"

namespace atmel
{
    class gpio :
            public gpio_handle
    {   
        public: 
            explicit gpio(const uint8_t pin) noexcept :
                gpio_handle(pin)
            { }

            gpio(const gpio&) = delete;
            //auto operator=(const gpio&) = delete; // not working for some reason

            gpio(gpio&& other) noexcept :
                gpio_handle(other.pin)
            {
                other.pin = -1;
            }
    };
}