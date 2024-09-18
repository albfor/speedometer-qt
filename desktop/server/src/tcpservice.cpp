#include <cstdlib>
#include <thread>
#include <cstring>

#include "tcpservice.h"

TCPService::TCPService() : COMService(), end{false}
{
  /* ----- create server ------- */
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    std::cerr << "Socket failed\n";
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)))
  {
    std::cerr << "Setsockopt failed\n";
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(Setting::Server::PORT);

  if (bind((server_fd), reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0)
  {
    std::cerr << "Bind failed\n";
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  /* ----- connection thread init ------- */
  std::thread(&TCPService::handle_connection, this).detach();
}

void *TCPService::handle_connection(void *arg)
{
  TCPService *service = static_cast<TCPService *>(arg);

  while (!service->end)
  {
    if (listen(service->server_fd, 3) < 0)
    {
      std::cerr << "Listen failed\n";
      close(service->server_fd);
      exit(EXIT_FAILURE);
    }
    std::cout << "Server is listening on port " << Setting::Server::PORT << std::endl;
    if ((service->client_fd =
             accept(service->server_fd, reinterpret_cast<struct sockaddr *>(&service->address),
                    reinterpret_cast<socklen_t *>(&service->addrlen))) < 0)
    {
      std::cerr << "Accept failed\n";
      continue;
    }

    std::cout << "Connection accepted\n";

    service->run();
    close(service->client_fd);
  }

  return nullptr;
}

void TCPService::run()
{
  while (!end)
  {
    uint8_t temp_buffer[sizeof(buffer)];

    {
      std::lock_guard<std::mutex> lock(mutex);
      std::memcpy(temp_buffer, buffer, sizeof(buffer));
    }

    if (::send(client_fd, temp_buffer, sizeof(temp_buffer), 0) == -1)
    {
      std::cerr << "Failed to send buffer\n";
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(Setting::INTERVAL_MS));
  }
}
