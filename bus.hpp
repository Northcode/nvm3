#pragma once
#include "head.hpp"
#include "device.hpp"

class bus
{
  std::vector<std::unique_ptr<device>> devices{5};
public:
  int register_device(device& device_ptr);

  void unregister_device(int index);

  bool bus_width(int index);

  byte inb(int index);
  maddr indw(int index);

  void out(int index, byte data);
  void out(int index, maddr data);
};

bus BUS{};
