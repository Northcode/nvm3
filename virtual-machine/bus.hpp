#pragma once
#include "head.hpp"
#include "device.hpp"

class bus
{
  std::vector<std::shared_ptr<device>> devices;
public:
  bus();
  ~bus();

  int register_device(std::shared_ptr<device> dev);

  void unregister_device(int index);

  void free_devices();

  bool bus_width(int index);

  byte inb(int index);
  maddr indw(int index);

  void out(int index, byte data);
  void out(int index, maddr data);
};

bus::bus() {
  devices = std::vector<std::shared_ptr<device>>(0);
  if (DEBUG_OUT)
    std::cout << "const bus " << this << std::endl;
}

bus::~bus() {
  if (DEBUG_OUT)
    std::cout << "dest bus " << this << std::endl;
}

void bus::free_devices() {
  for (auto p : devices)
    p.reset();
  devices.clear();
}

int bus::register_device(std::shared_ptr<device> dev) {
  devices.push_back(dev);
  return devices.size() - 1;
}

void bus::unregister_device(int index) {
  devices[index].reset();
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
  if(DEBUG_OUT)
    std::cout << "writing to device " << index << " data: " << data << std::endl;

  if(devices.size() <= index) {
    if(DEBUG_OUT)
      std::cout << "ERROR: device does not exist! device list length " << devices.size() << std::endl;
    return;
  } else
    if(DEBUG_OUT)
      std::cout << "device should exist device list length " << devices.size() << std::endl;
  devices[index]->recieve(data);
}
