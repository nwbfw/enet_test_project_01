#include <stdexcept>
#include <chrono>
#include <iostream>
#include <thread>
#include "MyServer.h"
#include "Utils.h"

MyServer::MyServer(const int port) {
  address.host = ENET_HOST_ANY;
  address.port = port;
  server = enet_host_create(&address, 32, 2, 0, 0);
  if (!server) throw std::runtime_error("Server creation failed.");
}

MyServer::~MyServer() {
  enet_host_destroy(server);
}

ENetHost * MyServer::getHost() const {
  return server;
}

void MyServer::run() {
  std::cout << "Listening on port " << server->address.port <<"...\n";
  while (true) {
    ENetEvent event{};
    while (true) {
      const int result = enet_host_service(server, &event, 0);
      if (result > 0) handleEvent(&event);
      if (result == 0) break;
      if (result < 0) throw std::runtime_error("Connection Error.");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void MyServer::handleEvent(ENetEvent* event) {
  switch (event->type) {
    case ENET_EVENT_TYPE_NONE:
      break;
    case ENET_EVENT_TYPE_CONNECT:
      std::cout << "A client has connected to the server: "
                << Utils::getIPAddressAsString(&event->peer->address)
                << ':' << event->peer->address.port << '\n';
      break;
    case ENET_EVENT_TYPE_DISCONNECT:
      std::cout << "A client has disconnected from the server: "
                << Utils::getIPAddressAsString(&event->peer->address)
                << ':' << event->peer->address.port << '\n';
                break;
    case ENET_EVENT_TYPE_RECEIVE:
      std::cout << "Received a data packet from client "
                << Utils::getIPAddressAsString(&event->peer->address)
                << ':' << event->peer->address.port << '\n'
                << "Packet data: ";
      std::string data_string{};
      for (int i = 0; i < event->packet->dataLength; i++) {
        data_string.push_back(event->packet->data[i]);
      }
      std::cout << data_string <<'\n';
      enet_packet_destroy(event->packet);

      const auto peer = event->peer;
      const std::string answer = "Received message \"" + data_string + "\".";
      const auto response = answer.c_str();
      enet_peer_send(
        peer,
        0,
        enet_packet_create(
          response,
          answer.size(),
          0
          )
        );
      break;
  }
  std::cout << "Listening on port " << server->address.port <<"...\n";
}