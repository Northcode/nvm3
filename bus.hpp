#pragma once
#include "head.hpp"
#include "device.hpp"

class bus
{
  vector<unique_ptr<device>> devices;
public:
  bus();

  int register_device(unique_ptr<device> device_ptr);

  void unregister_device(int index);

  bool bus_width(int index);

  byte inb(int index);
  maddr indw(int index);

  void out(int index, byte data);
  void out(int index, maddr data);
};
