#include "serialportservice.h"
#include <QDebug>

void SerialPortService::run()
{
    QSerialPort serialPort;
    serialPort.setPortName("/dev/ttyUSB0");
    serialPort.setBaudRate(Setting::SerialPort::BAUD_RATE);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    serialPort.setParity(QSerialPort::NoParity);

    while (!end)
    {
        if (connected && serialPort.isWritable())
        {
            uint8_t temp_buffer[sizeof(buffer)]{0};
            // Lock the buffer before accessing it
            {
                std::scoped_lock<std::mutex> lock(buffer_mutex);
                std::memcpy(temp_buffer, buffer, sizeof(buffer));
            }

            QByteArray data(reinterpret_cast<const char *>(temp_buffer), sizeof(buffer));
            // Send the data in the buffer

            if (serialPort.write(data) != sizeof(buffer))
            {
                qDebug() << "connection lost: " << serialPort.errorString() << "\n";
                connected = false;
                serialPort.close();
            }
            else
            {
                if (!serialPort.flush())
                {
                    qDebug() << "connection lost: " << serialPort.errorString() << "\n";
                    connected = false;
                    serialPort.close();
                }
            }
        }
        else
        {
            if (serialPort.open(QIODevice::WriteOnly))
            {
                qDebug() << "Connected on port: " << serialPort.portName();
                connected = true;
            }
            else
            {
                sleep(1);
                connected = false;
            }
        }

        // Sleep for 20 milliseconds before the next send
        msleep(Setting::INTERVAL_MS);
    }
}

bool SerialPortService::is_connected()
{
    return connected;
}
