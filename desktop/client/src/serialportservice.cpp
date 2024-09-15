#include "serialportservice.h"
#include "setting.h"
#include <QDebug>
#include <QSerialPort>

void SerialPortService::run()
{
    QSerialPort serial;
    serial.setPortName(Setting::SerialPort::CLIENT_PORT);
    serial.setBaudRate(Setting::SerialPort::BAUD_RATE);
    serial.setDataBits(QSerialPort::Data8);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setParity(QSerialPort::NoParity);

    while (!end)
    {
        if (connected && serial.isOpen() && serial.isReadable())
        {
            serial.clear();
            uint8_t temp_buffer[sizeof(buffer)]{0};
            if (serial.waitForReadyRead(Setting::INTERVAL_MS * 2)) // just something larger than INTERVAL_MS
            {
                if (sizeof(buffer) == serial.read(reinterpret_cast<char *>(temp_buffer), sizeof(buffer)))
                {
                    connected = true;
                    // Read success
                    std::lock_guard<std::mutex> lock(buffer_mutex);
                    std::memcpy(buffer, temp_buffer, sizeof(buffer));
                }
                else
                {
                    // Read fail
                    qDebug() << "Disconnected: Failed to read";
                    connected = false;
                    serial.close();
                }
            }
            else
            {
                qDebug() << "Disconnected: Time out error";
                connected = false;
                serial.close();
            }
        }
        else
        {
            if (serial.open(QIODevice::ReadOnly))
            {
                qDebug() << "Port open: " << serial.portName() << "\n";
                msleep(500); // give the port time to open
                connected = true;
            }
            else
            {
                connected = false;
            }
        }
        msleep(Setting::INTERVAL_MS);
    }

    if (serial.isOpen())
    {
        serial.close();
    }
}

bool SerialPortService::get_connection_state()
{
    return connected;
}
