#pragma once
#include "head.hpp"

class device
{
public:
  virtual void recieve(byte data);
  virtual void recieve(maddr data);
  virtual byte send_byte();
  virtual maddr send_int();

  virtual bool bus_width();
};
