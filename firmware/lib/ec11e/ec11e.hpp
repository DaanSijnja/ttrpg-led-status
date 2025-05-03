#pragma once

#include <Arduino.h>


template<class GPIO>
class ec11e {
    public:

        inline constexpr ec11e(
            GPIO pin_a,
            GPIO pin_b,
            GPIO button
        ) noexcept : 
            pin_a(pin_a),
            pin_b(pin_b),
            button(button),
            encoder_value(0),
            button_value(false)
        {}

        auto init() -> void {
            pin_a->set_mode(INPUT);
            pin_b->set_mode(INPUT);
            button->set_mode(INPUT);
        }

        auto update_encoder() -> void {
            const bool current_state = pin_a->get();       

            if(current_state)
            {
                if(pin_b->get() != current_state){
                    encoder_value++;
                } else {
                    encoder_value--;
                }
            }
        }
        
        [[nodiscard]] auto get_encoder_value() -> int16_t {
            return encoder_value;
        }

        auto set_encoder_value(int16_t value) -> void {
            encoder_value = value;
        }

        auto update_button() -> void {
            button_value = button->get();
        }

        [[nodiscard]] auto get_button_value() -> bool {
            return button_value;
        }

        auto set_button_value(bool value) {
            button_value = value;
        }

    private:
        GPIO pin_a;
        GPIO pin_b;
        GPIO button;

        int16_t encoder_value;
        bool button_value;
};  