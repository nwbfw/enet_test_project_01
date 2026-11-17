#ifndef ENET_TEST_PROJECT_01_UTILS_H
#define ENET_TEST_PROJECT_01_UTILS_H

#include <string>
#include "enet/enet.h"

class Utils {
public:
  static std::string getIPAddressAsString(const ENetAddress* e);
};

#endif //ENET_TEST_PROJECT_01_UTILS_H