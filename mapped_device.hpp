#pragma once
#include "head.hpp"
#include "device.hpp"

class mapped_device : public device
{
public:
  maddr address{0};
  size_t size{0};

  virtual void on_read(maddr, byte) {}
  virtual void on_read_dw(maddr, maddr) {}

  virtual void on_write(maddr, byte) {}
  virtual void on_write_dw(maddr, maddr) {}
};
