#include "main.hpp"

#ifndef databridge_h
#define databridge_h

struct port_c
{
  port_c() {
  }

  virtual void send(byte val) { std::cout << "dummy port got: " << (int)val << std::endl; }
  virtual void send(dword val) { std::cout << "dummy port got: " << (int)val << std::endl; }
  virtual byte get_b() { std::cout << "dummy port sending: 0" << std::endl; return 0; }
  virtual dword get_dw() { std::cout << "dummy port sending: 0" << std::endl; return 0; }
};

struct databridge : public device
{
  std::vector<std::shared_ptr<port_c>> ports;

  databridge();

  void register_port(byte port, std::shared_ptr<port_c> p);

  void out_b(byte port, byte value);

  void out_dw(byte port, dword value);

  byte in_b(byte port);

  dword in_dw(byte port);
};

#endif
