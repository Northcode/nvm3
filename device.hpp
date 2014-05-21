#pragma once
#include "head.hpp"

class device
{
public:
  virtual void recieve(byte data);
  virtual void recieve(int data);
  virtual byte sendbyte();
  virtual void sendint();

  virtual bool bus_width();
};
