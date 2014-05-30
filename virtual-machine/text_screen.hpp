#pragma once
#include "head.hpp"
#include "ram.hpp"
#include "mapped_device.hpp"

class text_device : public mapped_device
{
public:
  std::shared_ptr<ram> memory;

  text_device();
  ~text_device();

  void redraw();

  void on_write(maddr, byte);

  void recieve(byte);
  void recieve(maddr);
  byte send_byte();

  bool bus_width();
};

text_device::text_device() {
  size = 80*10;
  if (DEBUG_OUT)
    std::cout << "const text dev " << this << std::endl;
}

text_device::~text_device() {
  if (DEBUG_OUT)
    std::cout << "dest text dev " << this << std::endl;
}

void text_device::redraw() {
  gotoxy(0,0);
  for(int i = 0; i < this->size; i++) {
    if(i % 80 == 0)
      gotoxy(0,i/80);
    std::cout << (char)memory->read(this->address + i);
  }
}

void text_device::on_write(maddr i, byte data) {
  gotoxy(i % 80, i / 80);
  std::cout << (char)data;
}

void text_device::recieve(byte data) {
  if(data == 0) {
    redraw();
  }
}

void text_device::recieve(maddr data) {
  address = data;
}

byte text_device::send_byte() {
  char b{' '};
  std::cin >> b;
  return (byte)b;
}

bool text_device::bus_width() {
  return false;
}
