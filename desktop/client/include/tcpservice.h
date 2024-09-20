#ifndef TCPCOM_H
#define TCPCOM_H

#include "comservice.h"

#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <atomic>

class TCPService : public COMService
{
private:
    int client_fd;
    struct sockaddr_in server_address;
    std::atomic<bool> connected;
    std::atomic<bool> end;

public:
    TCPService();
    ~TCPService()
    {
        end = true;
    }
    bool connect_to_server();
    void run() override;
    bool is_connected() override;
};

#endif
