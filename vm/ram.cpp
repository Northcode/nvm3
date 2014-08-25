#include "main.hpp"

#ifndef RAM_C
#define RAM_C

memory::memory() {
  data = std::vector<byte>(MEMORY_SIZE);
  frames = std::vector<mem_frame>(MEMORY_SIZE/FRAME_SIZE);
  mapped_devices = std::vector<std::shared_ptr<mapped_device>>();
  initialized = true;
}

void memory::mapped_read(dword address, byte data) {
  for(auto dev : mapped_devices)
    if(address >= dev->address && address < dev->address + dev->size)
      dev->on_read_b(address,data);
}

void memory::mapped_read(dword address, dword data) {
  for(auto dev : mapped_devices)
    if(address >= dev->address && address < dev->address + dev->size)
      dev->on_read_dw(address,data);
}

void memory::mapped_write(dword address, byte data) {
  for(auto dev : mapped_devices)
    if(address >= dev->address && address < dev->address + dev->size)
      dev->on_write(address,data);
}

void memory::mapped_write(dword address, dword data) {
  for(auto dev : mapped_devices)
    if(address >= dev->address && address < dev->address + dev->size)
      dev->on_write(address,data);
}

void memory::map_device(std::shared_ptr<mapped_device> dev) {
  mapped_devices.push_back(dev);
}

byte memory::read_b(dword address) {
  if(address > data.size())
    return 0;
  mapped_read(address,data[address]);
  return data[address];
}

dword memory::read_dw(dword address) {
  if(address + 3 > data.size())
    return 0;
  dword res = (data[address + 3] << 24 | data[address + 2] << 16 | data[address + 1] << 8 | data[address]);
  mapped_read(address,res);
  return res;
}

void memory::write(dword address, byte value) {
  if(address > data.size())
    return;
  data[address] = value;
  mapped_write(address,value);
}

void memory::write(dword address, dword value) {
  if(address + 3 > data.size())
    return;
  data[address + 3] = value >> 24 & 0x000000ff;
  data[address + 2] = value >> 16 & 0x000000ff;
  data[address + 1] = value >>  8 & 0x000000ff;
  data[address + 0] = value       & 0x000000ff;
  mapped_write(address,value);
}

void memory::write(dword address, long value) {
  if(address + 7 > data.size())
    return;
  data[address + 7] = value >> 8*7 & 0x00000000000000ff;
  data[address + 6] = value >> 8*6 & 0x00000000000000ff;
  data[address + 5] = value >> 8*5 & 0x00000000000000ff;
  data[address + 4] = value >> 8*4 & 0x00000000000000ff;
  data[address + 3] = value >> 8*3 & 0x00000000000000ff;
  data[address + 2] = value >> 8*2 & 0x00000000000000ff;
  data[address + 1] = value >> 8*1 & 0x00000000000000ff;
  data[address + 0] = value        & 0x00000000000000ff;
}

int memory::getlol() {
  return data.size();
}

byte* memory::getD() {
  return data.data();
}
#endif
