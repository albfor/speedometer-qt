#include "tcpservice.h"
#include <thread>
#include <chrono>

TCPService::TCPService() : COMService(), connected{false}, end{false}
{
    std::cout << "Initializing tcp service\n";

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(Setting::Server::PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
    {
        std::cerr << "Invalid address\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Connecting to server...\n";
    std::thread(&TCPService::run, this).detach();
}

void TCPService::run()
{
    while (!end)
    {
        if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            std::cerr << "Socket creation failed\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        {
            std::cerr << "Connection failed\n";
            close(client_fd);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        else
        {
            std::cout << "Connected\n";
            connected = true;
        }

        while (!end && connected)
        {
            uint8_t temp_buffer[sizeof(buffer)];
            int number_of_bytes_read = read(client_fd, temp_buffer, sizeof(temp_buffer));

            if (number_of_bytes_read <= 0)
            {
                std::cerr << "Failed to read buffer\n";
                connected = false;
                close(client_fd);
            }
            else
            {
                std::lock_guard<std::mutex> lock(buffer_mutex);
                std::memcpy(buffer, temp_buffer, sizeof(buffer));
            }
            std::this_thread::sleep_for(std::chrono::microseconds(20));
        }
    }
    if (client_fd >= 0)
    {
        close(client_fd);
        client_fd = -1;
    }
}

bool TCPService::is_connected() { return connected; }
