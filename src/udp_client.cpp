#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <vector>

void sendVectorAsUDPClient(const std::vector<double>& cpuLoadVector) {

  //create socket
  int socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_udp < 0) {
    perror("Socket creation failed");
    return;
  }

  //add parameters to network server
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(1234);
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

  //send message (vector)
  size_t data_size = cpuLoadVector.size() * sizeof(double);
  sendto(socket_udp, cpuLoadVector.data(), data_size, 0,
         (struct sockaddr*)&server_address, sizeof(server_address));

  std::cout << "Vector sent." << std::endl;

  close(socket_udp);
  return;
}