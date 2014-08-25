#pragma once
#include "head.hpp"
#include "ram.hpp"
#include "mapped_device.hpp"
#include "screen.hpp"

class text_device : public mapped_device
{
public:
  std::shared_ptr<ram> memory;

  text_device();
  ~text_device();

  void redraw();
  void setpos(int,int);

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

void text_device::setpos(int x, int y) {
  if(DEBUG_OUT)
    std::cout << "setting position x: " << x << ", y: " << y << std::endl;
  gotoxy(x,y);
}

void text_device::redraw() {
  setpos(0,0);
  for(int i = 0; i < this->size; i++) {
    if(i % 80 == 0)
      std::cout << std::endl;
    std::cout << memory->read(this->address + i);
  }
  if(DEBUG_OUT)
    std::cout << "redrawing screen" << std::endl;
}

void text_device::on_write(maddr i, byte data) {
  maddr reladr = i - address;
  if(DEBUG_OUT)
    std::cout << "data " << (int)data << " written to address " << i << std::endl;
  setpos(reladr % 80 + 1, reladr / 80);
  std::cout << (char)data;
}

void text_device::recieve(byte data) {
  if(DEBUG_OUT)
    std::cout << "TXTSCRN Recieved byte " << data << std::endl;
  if(data == 0) {
    redraw();
  } else {
    std::cout << (char)data;
  }
}

void text_device::recieve(maddr data) {
  present = true;
  address = data;
  if(DEBUG_OUT)
    std::cout << "TEXT_SCREEN address mapped to: " << address << std::endl;
}

byte text_device::send_byte() {
  char b{' '};
  std::cin >> b;
  return (byte)b;
}

bool text_device::bus_width() {
  return false;
}
