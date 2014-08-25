#include "device.hpp"
#include "main.hpp"

#ifndef MAPPED_DEVICE_H
#define MAPPED_DEVICE_H

struct mapped_device : public device
{
  dword address;
  dword size;

  virtual void on_read_b(dword address,byte value);
  virtual void on_read_dw(dword address,dword value);

  virtual void on_write(dword address, byte data);
  virtual void on_write(dword address, dword data);
};
#endif
