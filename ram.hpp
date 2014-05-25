#pragma once
#include "heap.hpp"
#include "device.hpp"

class ram : public device
{
  std::vector<byte> data{RAM_SIZE};
public:

  byte read(maddr address);
  int readInt(maddr address);

  void write(maddr address, byte data);
  void write(maddr address, int data);
};

byte ram::read(maddr address) {
  return data[address];
}

int ram::readInt(maddr address) {
  return (data[address + 3] << 24 | data[address + 2] << 16 | data[address + 1] << 8 | data[address])
}

void ram::write(maddr address, byte data) {
  data[address] = data;
}

void ram::write(maddr address, int data) {
  write(address, data >> 24 & 0xff);
  write(address, data >> 16 & 0xff);
  write(address, data >>  8 & 0xff);
  write(address, data       & 0xff);
}
