#pragma once

#include "gpio_handle.hpp"

namespace arduino
{
    class gpio :
            public gpio_handle
    {   
        public: 
            explicit gpio(const uint8_t pin) noexcept :
                gpio_handle(pin)
            { }

            gpio(const gpio&) = delete;
            gpio& operator=(const gpio&) = delete; 

            gpio(gpio&& other) noexcept :
                gpio_handle(other.pin)
            {
                other.pin = -1;
            }
    };
}