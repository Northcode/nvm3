#include "main.hpp"

#ifndef DATABRIDGE_C
#define DATABRIDGE_C

databridge::databridge() : device() {
  ports = std::vector<std::shared_ptr<port_c>>(DATA_BRIDGE_SIZE);
  initialized = true;
}

void databridge::register_port(byte port, std::shared_ptr<port_c> p) {
  ports[port] = p;
}

void databridge::out_b(byte port, byte value) {
  if(port > ports.size())
    return;
  if(ports[port] == NULL)
  { std::cout << "PORT INVALID, ABORT" << std::endl; return; }
  ports[port]->send(value);
}

void databridge::out_dw(byte port, dword value) {
  if(port > ports.size())
    return;
  ports[port]->send(value);
}

byte databridge::in_b(byte port) {
  if(port > ports.size())
    return 0;
  return ports[port]->get_b();
}

dword databridge::in_dw(byte port) {
  if(port > ports.size())
    return 0;
  return ports[port]->get_dw();
}
#endif
