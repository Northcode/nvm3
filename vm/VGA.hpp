#include "main.hpp"

#ifndef VGA_device
#define VGA_device

struct VGA_dev : public device
{
  std::vector<byte> vram;
  dword pos;

  VGA_dev();

  void power();
  void tick();

  void redraw();

  void draw();

  void display();
};

struct VGA_port : public port_c
{
  VGA_dev* dev;

  VGA_port(VGA_dev* dev) : port_c() {
    this->dev = dev;
  }

  void send(byte b) {
    dev->vram[dev->pos] = b;
  }

  void send(dword dw) {
    dev->pos = dw;
  }
};

#endif
