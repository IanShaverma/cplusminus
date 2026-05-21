#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <vector>

int main() {
  int socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in server_address, client_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(1234);
  server_address.sin_addr.s_addr = INADDR_ANY;

  bind(socket_udp, (struct sockaddr*)&server_address, sizeof(server_address));

  char buffer[1024];
  socklen_t address_lenght = sizeof(client_address);

  ssize_t received =
      recvfrom(socket_udp, buffer, sizeof(buffer), 0,
               (struct sockaddr*)&client_address, &address_lenght);

  if (received > 0) {
    double* double_ptr = reinterpret_cast<double*>(buffer);
    size_t count = received / sizeof(double);
    std::vector<double> receivedCpuLoadVector(double_ptr, double_ptr + count);
    std::cout << "Received: ";
    for (double v : receivedCpuLoadVector)
      std::cout << v << " ";
    std::cout << std::endl;
  }
  close(socket_udp);
  return 0;
}