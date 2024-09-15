#include "comservice.h"
#include "setting.h"
#include <climits>
#include <mutex>

// Extracts a portion of the buffer starting at a specific bit position and of a specific length
uint8_t COMService::extract(uint32_t start, uint32_t length) const
{
    // Lock the mutex to ensure thread safety while accessing the buffer
    std::lock_guard<std::mutex> lock(buffer_mutex);

    uint8_t data{0}; // Initialize the data to store the extracted bits
    // Check if the start position and length fit within the buffer size
    if (start + length <= sizeof(buffer) * CHAR_BIT)
    {
        int pos = start % CHAR_BIT;   // Determine the bit position within the current byte
        int index = start / CHAR_BIT; // Determine the byte index in the buffer
        // Loop through each bit to extract
        for (std::size_t i = 0; i < length; i++)
        {
            // Extract the bit at the current position
            uint8_t bit = (buffer[index] >> pos) & 1;
            // Set the corresponding bit in the data variable
            data |= (bit << i);
            // Move to the next bit position
            if (++pos == CHAR_BIT)
            {
                pos = 0; // Reset bit position
                index++; // Move to the next byte
            }
        }
    }
    return data; // Return the extracted data
}

// Retrieves the speed value from the buffer
uint8_t COMService::get_speed() const
{
    return extract(signal["speed"].start, signal["speed"].length);
}

// Retrieves the temperature value from the buffer
int8_t COMService::get_temperature() const
{
    int8_t val = extract(signal["temperature"].start, signal["temperature"].length);

    // Check if the value is negative (using the sign bit)
    if (val & (1 << (signal["temperature"].length - 1)))
    {
        // Set the sign bit (most significant bit) for negative values
        val |= ~((1 << signal["temperature"].length) - 1); // Extend sign to the entire byte
    }

    return val; // Return the temperature value
}

// Retrieves the battery level value from the buffer
uint8_t COMService::get_battery_level() const
{
    return extract(signal["battery_level"].start, signal["battery_level"].length);
}

// Retrieves the state of the light signal from the buffer
Setting::turn_signal_state COMService::get_turn_signal_state() const
{
    uint8_t sig = extract(signal["light_signal_left"].start, signal["light_signal_left"].length) +
                  (extract(signal["light_signal_right"].start, signal["light_signal_right"].length) << 1);
    return static_cast<Setting::turn_signal_state>(sig);
}
