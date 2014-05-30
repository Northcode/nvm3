#pragma once
#include "head.hpp"

class device
{
public:
  device() {}

  virtual void recieve(byte data) {}
  virtual void recieve(maddr data) {}
  virtual byte send_byte() { return 0; }
  virtual maddr send_int() { return 0; }

  virtual bool bus_width() { return false; }
};
