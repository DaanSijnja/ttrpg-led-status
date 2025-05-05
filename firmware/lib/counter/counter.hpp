#pragma once

namespace counter
{
    typedef unsigned long long int counter_t;

    class clock {
        public:
            constexpr clock() noexcept {}

            auto get_count() -> counter_t {
                return count;
            }

            auto up_count(const int value) -> void {
                count += value;
            }
            
        private:
            counter_t count = 0;
    };

    class compare {
         public:
            constexpr compare(clock *c, const unsigned int interval) noexcept :
            c(c),
            interval(interval),
            last_interval(0)
            {}

            auto compare_count(const bool reset = true) noexcept -> bool {
                const counter_t value = c->get_count();
                if( value - last_interval >= interval ) {
                    if(reset){ last_interval = value; }
                    return true;
                }
                return false;
            }

            auto set_interval(const unsigned int interval) noexcept -> void {
                this->interval = interval;
            }

            auto set_last_interval(const counter_t last_interval) noexcept -> void {
                 this->last_interval = last_interval;
            }

            auto set_last_interval_to_current() -> void {
                 this->last_interval = c->get_count();
            }

        private:
            clock *c;
            unsigned int interval;
            counter_t last_interval;
    };
} 
