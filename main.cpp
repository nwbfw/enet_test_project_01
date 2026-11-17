#include <iostream>

#include "MyClient.h"
#include "MyServer.h"
#include "enet/enet.h"

static void initLibraries();
void exitProgram(int);

int main(int argc, char** argv) {
  try {
    initLibraries();

    if (argc < 3) throw std::runtime_error("args");

    const std::string mode = argv[1];
    if (mode == "server") {
      const int port = std::stoi(argv[2]);
      MyServer srv{port};
      srv.run();
    }
    else if (mode == "client") {
      if (argc < 4) throw std::runtime_error("args");
      const std::string address = argv[2];
      const std::string port = argv[3];

      MyClient clnt{};

      ENetAddress target_address{};
      enet_address_set_host_ip(&target_address, address.c_str());
      target_address.port = std::stoi(port);

      clnt.connectToServer(target_address);
      clnt.run();
    }
    else throw std::runtime_error("args");

  }
  catch (std::exception& e) {
    if (std::string{e.what()} == "args")
      std::cout << R"(usage: "client <ip> <port>" or "server <port>")";
    else std::cout << "uncaught exception: " << e.what();
    exitProgram(-1);
  }

  exitProgram(0);
}

static void initLibraries() {
  if (enet_initialize() != 0) throw std::runtime_error("ENET initialization failed.");
}

void exitProgram(const int exit_code) {
  enet_deinitialize();
  std::exit(exit_code);
}