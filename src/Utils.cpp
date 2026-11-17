#include "Utils.h"

std::string Utils::getIPAddressAsString(const ENetAddress* e) {
  char res[15];
  enet_address_get_host_ip(e, res, 15);
  return std::string{res};
}