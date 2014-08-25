#include "main.hpp"

#ifndef RAM_H
#define RAM_H

struct mem_frame
{
  dword address;
  bool is_free;
};

struct memory : public device
{
  std::vector<byte> data;

  int A;

  memory();

  //memory frames
  std::vector<mem_frame> frames;

  std::shared_ptr<mem_frame> get_free_frame();
  std::shared_ptr<mem_frame> get_frame(dword index);

  //memory mapped devices
  std::vector<std::shared_ptr<mapped_device>> mapped_devices;

  void mapped_read(dword address, byte data);
  void mapped_read(dword address, dword data);
  void mapped_write(dword address, byte data);
  void mapped_write(dword address, dword data);

  void map_device(std::shared_ptr<mapped_device> device);

  //read
  byte read_b(dword address);
  dword read_dw(dword address);

  //write
  void write(dword address, byte data);
  void write(dword address, dword data);

  //for writing instructions
  void write(dword address, long data);

  int getlol();
  byte* getD();
};
#endif
