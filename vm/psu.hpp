#include "main.hpp"

#ifndef PSU_H
#define PSU_H

//power control unit
struct psu : public device
{
  psu();

  void power();

  void poweroff();

  void send(byte);
  void send(dword);
};

#endif
