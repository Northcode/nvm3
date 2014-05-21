#pragma once
#include "heap.hpp"
#include "device.hpp"

class ram : device
{
  std::vector<byte> data;
  maddr pos;
public:
  ram() {
    data = std::vector<byte>(RAM_SIZE);
  }

  byte read(maddr address);
  int readInt(maddr address);

  void write(maddr address, byte data);
  void write(maddr address, int data);
};
