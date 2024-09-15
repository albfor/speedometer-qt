#ifndef COMSERVICE_H
#define COMSERVICE_H

#include "setting.h"
#include <cstdint>
#include <climits>
#include <mutex>

/// @brief Abstract class for communication for the server.
class COMService
{
private:
    Setting::Signal &signal{Setting::Signal::instance()};

    /// @brief Insert data into the buffer.
    /// @param data The data to insert.
    /// @param start Index of the bit where the data is to be stored.
    /// @param length number of bits needed for storage.
    /// @return True if insertion is successful.
    void insert(uint8_t data, uint32_t start, uint32_t length);

protected:
    std::mutex mutex;
    uint8_t buffer[Setting::Signal::BUFFER_SIZE]{0};

public:
    /// @brief Insert speed into the buffer.
    /// @param speed Speed in kph. Range [0:240]
    /// @return True if successful insertion.
    void set_speed(uint8_t speed);

    /// @brief Insert temperature into the buffer.
    /// @param temperature Temperature in Celcius. Range [-60:60]
    /// @return True if successful insertion.
    void set_temperature(int8_t temperature);

    /// @brief Insert battery level into the buffer.
    /// @param battery_level Battery level percentage. Range [0:100]
    /// @return True if successful insertion.
    void set_battery_level(uint8_t battery_level);

    /// @brief Insert status of the light signals
    /// @param light_signal_state Status of light signals. Can have the values OFF, LEFT, RIGHT, and WARNING
    /// @return True if successful insertion.
    void set_turn_signal(Setting::turn_signal_state signal);

    /// @brief Send the buffer via any given communication protocol in derived classes.
    virtual void run() = 0;
};

#endif
