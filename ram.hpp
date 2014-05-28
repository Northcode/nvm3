#pragma once
#include "head.hpp"
#include "device.hpp"

class ram : public device
{
  std::vector<byte> data = std::vector<byte>(RAM_SIZE);
public:

  byte read(maddr address);
  int read_int(maddr address);

  void write(maddr address, byte data);
  void write(maddr address, int data);
};

ram memory{};

byte ram::read(maddr address) {
  return data[address];
}

int ram::read_int(maddr address) {
  return (data[address + 3] << 24 | data[address + 2] << 16 | data[address + 1] << 8 | data[address]);
}

void ram::write(maddr address, byte data) {
  this->data[address] = data;
}

void ram::write(maddr address, int data) {
  write(address, data >> 24 & 0xff);
  write(address, data >> 16 & 0xff);
  write(address, data >>  8 & 0xff);
  write(address, data       & 0xff);
}
