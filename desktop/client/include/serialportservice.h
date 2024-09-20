#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "comservice.h"

#include <QThread>

class SerialPortService : public COMService, public QThread
{
    std::atomic<bool> end{false};
    std::atomic<bool> connected{false};

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

    bool is_connected() override;
};

#endif
