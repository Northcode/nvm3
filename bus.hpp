#pragma once
#include "head.hpp"
#include "device.hpp"

class bus
{
  std::vector<std::shared_ptr<device>> devices{5};
public:
  int register_device(std::shared_ptr<device> dev);

  void unregister_device(int index);

  bool bus_width(int index);

  byte inb(int index);
  maddr indw(int index);

  void out(int index, byte data);
  void out(int index, maddr data);
};

bus data_bus{};

int bus::register_device(std::shared_ptr<device> dev) {
  devices.push_back(dev);
  return devices.size() - 1;
}

void bus::unregister_device(int index) {
  devices.erase(devices.begin() + index);
}

bool bus::bus_width(int index) {
  return devices[index]->bus_width();
}

byte bus::inb(int index) {
  return devices[index]->send_byte();
}

maddr bus::indw(int index) {
  return devices[index]->send_int();
}

void bus::out(int index, byte data) {
  devices[index]->recieve(data);
}

void bus::out(int index, maddr data) {
  devices[index]->recieve(data);
}
