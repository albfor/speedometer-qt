#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QThread>
#include <QSerialPort>
#include <atomic>
#include <mutex>
#include "comservice.h"

class SerialPortService : public COMService, public QThread
{

    std::atomic<bool> end{false};
    std::atomic<bool> connected{false};
    std::mutex buffer_mutex;

    void run() override;

public:
    SerialPortService()
    {
        start();
    }
    ~SerialPortService()
    {
        end = true;
        wait();
    }

    bool get_connection_state();
};

#endif // SERIALPORT_H
