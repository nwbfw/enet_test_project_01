#ifndef ENET_TEST_PROJECT_01_MYSERVER_H
#define ENET_TEST_PROJECT_01_MYSERVER_H

#include "enet/enet.h"

class MyServer {
private:
  ENetAddress address{};
  ENetHost* server{nullptr};
public:
  ENetHost* getHost() const;
  MyServer() = delete;
  MyServer(int port);
  void run();
  ~MyServer();
private:
  void handleEvent(ENetEvent* event);
};

#endif //ENET_TEST_PROJECT_01_MYSERVER_H
