#ifndef COMSERVICE_H
#define COMSERVICE_H

#include "setting.h"
#include <cstdint>
#include <mutex>

/// @brief Abstract class for communication for the client.
class COMService
{
private:
    /// @brief Extract data from the buffer.
    /// @param start Index of the bit where the data is to be extracted.
    /// @param length number of bits to extract.
    /// @return Extracted data.
    uint8_t extract(uint32_t start, uint32_t length) const;

    Setting::Signal &signal{Setting::Signal::instance()};

protected:
    uint8_t buffer[Setting::Signal::BUFFER_SIZE]{};
    mutable std::mutex buffer_mutex;

public:
    /// @brief Extract speed from the buffer.
    /// @return Speed in kph. Range [0:240]
    uint8_t get_speed() const;

    /// @brief Extract temperature from the buffer.
    /// @return Temperature in Celsius. Range [-60:60]
    int8_t get_temperature() const;

    /// @brief Extract battery level from the buffer.
    /// @return Battery level percentage. Range [0:100]
    uint8_t get_battery_level() const;

    /// @brief Extract status of the light signals
    /// @return Status of light signals. Can have the values OFF, LEFT, RIGHT, and WARNING
    Setting::turn_signal_state get_turn_signal_state() const;

    /// @brief Receive the buffer via any given communication protocol in derived classes.
    virtual void run() = 0;

    /// @brief Return true if there is a connection.
    virtual bool is_connected() = 0;
};

#endif // COMSERVICE_H
