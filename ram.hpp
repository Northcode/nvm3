#pragma once
#include "head.hpp"
#include "device.hpp"
#include <algorithm>

struct mem_frame
{
  maddr address{0};
  bool is_free{false};
};

class ram : public device
{
  std::vector<byte> data = std::vector<byte>(RAM_SIZE);
  std::vector<mem_frame> frames = std::vector<mem_frame>(RAM_SIZE/PAGE_SIZE);
public:
  byte read(maddr address);
  int read_int(maddr address);

  void write(maddr address, byte data);
  void write(maddr address, maddr data);

  std::vector<mem_frame>::iterator find_frame();

  void free_frame(int index);

  void recieve(maddr data);
  maddr send_int();

  bool bus_width();
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

void ram::write(maddr address, maddr data) {
  write(address, data >> 24 & 0xff);
  write(address, data >> 16 & 0xff);
  write(address, data >>  8 & 0xff);
  write(address, data       & 0xff);
}

void ram::recieve(maddr data) {
  if(data == 0) {
    //"powering on"
    for(int i = 0; i < frames.size(); i++) {
      this->frames[i].address = PAGE_SIZE*i;
    }
  }
}

maddr ram::send_int() {
  return 0;
}

bool ram::bus_width() { return true; }

void ram::free_frame(int index) {
  this->frames[index].is_free = true;
}

std::vector<mem_frame>::iterator ram::find_frame() {
  return std::find_if(frames.begin(),frames.end(), [&](const mem_frame& f) { return f.is_free; });
}
