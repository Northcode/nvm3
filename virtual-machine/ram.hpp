#pragma once
#include "head.hpp"
#include "device.hpp"
#include "mapped_device.hpp"
#include <algorithm>

struct mem_frame
{
  maddr address{0};
  bool is_free{false};
};

class ram : public device
{
  std::vector<byte> data;
  std::vector<mem_frame> frames;
  std::vector<std::shared_ptr<mapped_device>> mapped_devices;
public:
  ram();

  ~ram();

  byte read(maddr);
  int read_int(maddr);

  void write(maddr, byte);
  void write(maddr, maddr);

  std::vector<mem_frame>::iterator find_frame();

  void free_frame(int);

  void map_device(std::shared_ptr<mapped_device>);
  void unmap_device(int);

  void recieve(maddr);
  maddr send_int();

  bool bus_width();
};

ram::ram() {
  if (DEBUG_OUT)
    std::cout << "const ram " << this << std::endl;
  data = std::vector<byte>(RAM_SIZE);
  frames = std::vector<mem_frame>(RAM_SIZE/PAGE_SIZE);
  mapped_devices = std::vector<std::shared_ptr<mapped_device>>();
}

ram::~ram() {
  if(DEBUG_OUT)
    std::cout << "dest ram" << this << std::endl;
  for(auto p : mapped_devices)
    p.reset();
  mapped_devices.clear();
}

byte ram::read(maddr address) {
  //std::cout << "reading byte at: " << address << std::endl;
  for (auto& d : mapped_devices) {
    if(d->address <= address && d->address + d->size >= address)
      d->on_read(address,data[address]);
  }
  return data[address];
}

int ram::read_int(maddr address) {
  int res = (data[address + 3] << 24 | data[address + 2] << 16 | data[address + 1] << 8 | data[address]);
  for (auto& d : mapped_devices) {
    if(d->address <= address && d->address + d->size >= address)
      d->on_read_dw(address,res);
  }
  return res;
}

void ram::write(maddr address, byte data) {
  for (auto& d : mapped_devices) {
    if(d->address <= address && d->address + d->size >= address)
      d->on_read(address,data);
  }
  this->data[address] = data;
}

void ram::write(maddr address, maddr data) {
  for (auto& d : mapped_devices) {
    if(d->address <= address && d->address + d->size >= address)
      d->on_write_dw(address,data);
  }
  this->data[address] = data >> 24 & 0xff;
  this->data[address] = data >> 16 & 0xff;
  this->data[address] = data >>  8 & 0xff;
  this->data[address] = data       & 0xff;
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

void ram::map_device(std::shared_ptr<mapped_device> dev) {
  mapped_devices.push_back(dev);
}

void ram::unmap_device(int index) {
  mapped_devices.erase(mapped_devices.begin() + index);
}
