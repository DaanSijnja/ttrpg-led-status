#pragma once

#include "common.hpp"

namespace settings {

    constexpr unsigned int COUNTER_FREQUENCY    = 1000;                            // Frequency in Hz of the counter clock used for logica
    constexpr unsigned int BLINK_FREQUENCY      = 2;                               // Frequency in Hz of the blinking ledbar default (will dynamicy change to simulate 0. values on the bar)
    constexpr unsigned int TOTAL_STEPS          = 10;                              // Total steps to be shown on the ledbar

    constexpr common::status_led_check status_led_index[] = {
                { 10 , common::color(1,0,0) }
            };

    constexpr size_t STATUS_LED_INDEX_SIZE = sizeof(status_led_index) / sizeof(status_led_index);
}