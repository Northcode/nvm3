#include "main.hpp"

#ifndef PSU_C
#define PSU_C

struct psu_poweroff_port : public port_c
{
  psu* dev;

  psu_poweroff_port(psu* dev) : port_c() { this->dev = dev; }

  void send(byte b) {
    std::cout << "POWER OFF SIGNAL RECIEVED!" << std::endl;
    dev->poweroff();
  }

  void send(dword d) {
    std::cout << "POWER OFF SIGNAL RECIEVED!" << std::endl;
    dev->poweroff();
  }
};

psu::psu() : device() {
  initialized = true;
}

void psu::power() {
  std::shared_ptr<psu_poweroff_port> ppp = std::make_shared<psu_poweroff_port>(this);
  machine->_databridge->register_port(0,ppp);
}

void psu::poweroff()
{
  machine->RN = false;
}

#endif
