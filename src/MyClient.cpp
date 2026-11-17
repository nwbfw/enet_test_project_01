#include <chrono>
#include <thread>
#include <stdexcept>
#include <iostream>
#include "MyClient.h"
#include "Utils.h"

void exitProgram(int exit_code);

MyClient::MyClient() {
  client = enet_host_create(nullptr, 1, 2, 0, 0);
  if (!client) throw std::runtime_error("Client creation failed.");
}

MyClient::~MyClient() {
  enet_host_destroy(client);
}

ENetHost * MyClient::getHost() const {
  return client;
}

void MyClient::connectToServer(const ENetAddress server_address) {
  connected_server = enet_host_connect(
    client,
    &server_address,
    2,
    0
  );
  std::cout << "Requesting connection to server "
            << Utils::getIPAddressAsString(&connected_server->address)
            << ':' << connected_server->address.port << '\n';
}

void MyClient::run() {
  int counter = 0;
  int value = 0;
  while (true) {
    ENetEvent event{};
    while (true) {
      const int result = enet_host_service(client, &event, 0);
      if (result > 0) handleEvent(&event);
      if (result == 0) break;
      if (result < 0) throw std::runtime_error("Connection Error.");
    }
    if (counter >= 500 && connected_to_server) {
      std::string message{"Hello Server " + std::to_string(value) + "!"};
      auto packet_to_send = enet_packet_create(
        message.c_str(),
        message.size(),
        0
      );
      enet_peer_send(connected_server, 0, packet_to_send);
      counter = 0;
      value++;
    }
    counter++;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void MyClient::handleEvent(ENetEvent *event) {
  switch (event->type) {
    case ENET_EVENT_TYPE_NONE:
      break;
    case ENET_EVENT_TYPE_CONNECT: {
      auto connected_peer = event->peer;
      std::cout << "Connection established to server "
                << Utils::getIPAddressAsString(&connected_peer->address)
                << ':' << connected_peer->address.port << '\n';
      if (connected_peer == connected_server) connected_to_server = true;
      else {
        std::cout << "!!!Server/Peer mismatch!!!";
        exitProgram(-1);
      }
    } break;
    case ENET_EVENT_TYPE_DISCONNECT:
      std::cout << "Disconnected from Server "
                << Utils::getIPAddressAsString(&event->peer->address)
                << ':' << event->peer->address.port << '\n';
      break;
    case ENET_EVENT_TYPE_RECEIVE:
      std::cout << "Received a data packet from server "
                << Utils::getIPAddressAsString(&event->peer->address)
                << ':' << event->peer->address.port << '\n'
                << "Packet data: ";
      std::string data_string{};
      for (int i = 0; i < event->packet->dataLength; i++) {
        data_string.push_back(event->packet->data[i]);
      }
      std::cout << data_string << '\n';
      enet_packet_destroy(event->packet);
      break;
  }
}