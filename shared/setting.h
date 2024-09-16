#ifndef SETTING_H
#define SETTING_H

#include <climits>
#include <map>
#include <string>

namespace Setting
{

enum turn_signal_state
{
    OFF,
    LEFT,
    RIGHT,
    WARNING
};

struct signal_value
{
    int min, max, start, length;
    constexpr signal_value(int _min = 0, int _max = 0, int _start = 0, int _length = 0)
        : min{_min}, max{_max}, start{_start}, length{_length}
    {
    }
};

struct signal_type
{
    const char *key;
    signal_value value;
};

template <size_t Size> static constexpr size_t sum(const signal_type (&arr)[Size], size_t index = 0, size_t result = 0)
{
    return index == Size ? result : sum(arr, index + 1, result + arr[index].value.length);
}

class Signal
{
    static constexpr signal_type list[] = {
        {"speed", {0, 240, 0, 8}},
        {"temperature", {-60, 60, 8, 7}},
        {"battery_level", {0, 100, 15, 7}},
        {"light_signal_left", {0, 1, 22, 1}},
        {"light_signal_right", {0, 1, 23, 1}},
    };

    std::map<std::string, signal_value> sig_map;

    Signal()
    {
        for (const auto &elem : list)
        {
            sig_map[elem.key] = elem.value;
        }
    }

    Signal(const Signal &) = delete;
    Signal &operator=(const Signal &) = delete;

public:
    static constexpr size_t BUFFER_SIZE{sum(list) / CHAR_BIT + (sum(list) % CHAR_BIT == 0 ? 0 : 1)};

    static Signal &instance()
    {
        static Signal instance;
        return instance;
    }

    signal_value &operator[](const std::string &key)
    {
        return sig_map[key];
    }
};

// Constexpr array to store signal configurations by name

constexpr unsigned int INTERVAL_MS{20};

namespace Server
{
constexpr int PORT{8080};
}

namespace Server_GUI
{
namespace Window_Size
{
constexpr int width{1000};
constexpr int height{250};
} // namespace Window_Size

namespace Label_Width
{
constexpr int width{60};
}
} // namespace Server_GUI

namespace SerialPort
{
constexpr int BAUD_RATE{9600};
constexpr char CLIENT_PORT[] = "/dev/ttyUSB0";
constexpr char SERVER_PORT[] = "/dev/ttyUSB1";
} // namespace SerialPort

} // namespace Setting

#endif // SETTING_H
