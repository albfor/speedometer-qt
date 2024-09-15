#ifndef TCPCOM_H
#define TCPCOM_H

#include "comservice.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <atomic>

class TCPService : public COMService
{
private:
    int server_fd;
    int client_fd;
    struct sockaddr_in address;
    static constexpr int opt = 1;
    int addrlen = sizeof(address);
    std::atomic<bool> end;

    static void *handle_connection(void *ptr);

public:
    ~TCPService()
    {
        close(server_fd);
        close(client_fd);
        end = true;
    }
    TCPService();

    void run() override;
};

#endif
