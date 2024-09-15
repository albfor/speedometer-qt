#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QThread>
#include "comservice.h"

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

    bool get_connection_state() override;
};

#endif
