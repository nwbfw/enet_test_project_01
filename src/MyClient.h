#ifndef ENET_TEST_PROJECT_01_MYCLIENT_H
#define ENET_TEST_PROJECT_01_MYCLIENT_H

#include "enet/enet.h"

class MyClient {
private:
  ENetHost* client{nullptr};
  ENetPeer* connected_server{nullptr};
  bool connected_to_server{false};
public:
  ENetHost* getHost() const;
  MyClient();
  void connectToServer(ENetAddress server_address);
  void run();
  ~MyClient();
private:
  void handleEvent(ENetEvent* event);
};

#endif //ENET_TEST_PROJECT_01_MYCLIENT_H