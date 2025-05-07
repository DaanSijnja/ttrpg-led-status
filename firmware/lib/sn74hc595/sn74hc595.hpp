#pragma once

/// @brief The firmware representation class for the sn74hc595 8-bit shift register chip
/// @tparam GPIO GPIO class wrapper
/// @tparam N Total chips connected in series
template <unsigned int N, class GPIO>
class sn74hc595 {
    public:
       
        /// @param ser The GPIO representation of the "SER" pin. aka serial data pin
        /// @param srclk The GPIO representation of the "SRCLK" pin. aka serrial clock pin
        /// @param rclk The GPIO representation of the "RCLK" pin. aka latch pin
        /// @param srclr <optional> The GPIO representation of the "SRCLR" pin. aka master clear pin
        /// @param oe <optional> The GPIO representation of the "OE" pin. aka output enable pin
        inline constexpr sn74hc595(
            GPIO ser,
            GPIO srclk,
            GPIO rclk,
            GPIO srclr = (GPIO)nullptr,
            GPIO oe    = (GPIO)nullptr
        ) noexcept :
            ser(ser),
            srclk(srclk),
            rclk(rclk),
            srclr(srclr),
            oe(oe)
        {}

        /// @brief initalizes the sn74hc595 chip and GPIO pins
        inline auto init() noexcept -> void {
            ser->set_dir(OUTPUT);
            srclk->set_dir(OUTPUT);
            rclk->set_dir(OUTPUT);

            if(srclr != (GPIO)nullptr)
                ser->set_dir(OUTPUT);

            if(oe != (GPIO)nullptr)    
                ser->set_dir(OUTPUT);
        }
        
        /// @brief 
        /// @param data 
        /// @param bit_order 
        /// @param number_of_bits 
        auto set_output(const uint8_t * data, const uint8_t bit_order, const unsigned int number_of_bits = 8 * N ) noexcept -> void {
            rclk->put(0);   

            uint8_t total_bytes = (number_of_bits / 8);
            if(number_of_bits > total_bytes*8){
                total_bytes += 1;
            }

            for(unsigned int i = 0; i < total_bytes; i++ ) {
                const unsigned int bit_remaining = (( number_of_bits - (i * 8) ) > 8) ? 8 : number_of_bits - (i * 8);
                
                if(bit_order == LSBFIRST)
                    shift_data_out(data[i], bit_order, bit_remaining);
                else
                    shift_data_out(data[( total_bytes - 1u) - i], bit_order, bit_remaining);
            }
            rclk->put(1);
        }
        
        /// @brief Sends the given 8-bit output data to the sn74hc595 chips outputs
        /// @param data 8-bit output value 
        /// @param bit_order the bit order which the data should be send to the sn74hc595 chip
        /// @param number_of_bits <optional, default = 8> total of bits needed to be send
        auto set_output(const uint8_t data, const uint8_t bit_order, const unsigned int number_of_bits = 8) noexcept -> void {
            rclk->put(0);
            shift_data_out(data,bit_order,number_of_bits);
            rclk->put(1);
        }
        
        /// @brief Sets the output enable ( only if the GPIO 'oe' was defined )
        /// @param value sets the output enable gpio to this value
        inline auto output_enable(const bool value) noexcept -> void {
            if(oe == (GPIO)nullptr) return;
            oe->put(!value);
        }

        /// @brief Clears the register with the master clear gpio pin ( only if the GPIO 'srclr' was defined )
        /// @param latch whenever the register clear also is latched to the outputs
        inline auto clear_register(const bool latch = false) noexcept -> void {
            if(srclr == (GPIO)nullptr) return;
            srclr->put(0);
            srclr->put(1);

            if(latch){
                rclk->put(1);
                rclk->put(0);
            }
        }


    private:
        const GPIO ser;                             
        const GPIO srclk;
        const GPIO rclk;
        const GPIO srclr;
        const GPIO oe;

        inline auto shift_data_out(const uint8_t data, const uint8_t bit_order, const unsigned int number_of_bits = 8) -> void {
            srclk->put(0);
            ser->put(0);
            for(uint8_t i = 0; i < number_of_bits; i++) {
                srclk->put(0);
                if(bit_order == LSBFIRST)
                    ser->put( !!( data & (1 << i) ) );
                else
                    ser->put( !!( data & (1 << ((number_of_bits - 1) - i)) ) );

                srclk->put(1); 
                ser->put(0);
            }
            srclk->put(0);
        }


};