#include "main.hpp"

#ifndef DEVICE_H
#define DEVICE_H

enum device_type
{
  proc,
  storage,
  bus
};

struct device
{
  vm* machine;

  bool initialized;

  device();

  virtual void tick() {}

  virtual void reset() {}

  virtual void power() {}
};
#endif
