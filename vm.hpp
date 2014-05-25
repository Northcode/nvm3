#pragma once
#include "head.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "media_device.hpp"

class vm
{
  bus data_bus{};

  std::unique_ptr<cpu> _cpu;

public:
  vm();

  void power();

  void reset();

  void insert_media(media_device& media);

  void add_device(device& dev);
};

vm::vm() {
  cpu __cpu{};
  _cpu = std::unique_ptr<cpu>(__cpu);
  data_bus.register_device(__cpu);
}

void vm::add_device(device& dev) {
  data_bus.register_device(dev);
}

void vm::power() {
  _cpu->execute();
}

void vm::reset() {
  _cpu->halt();
  _cpu->reset();
  _cpu->execute();
}

void vm::insert_media(media_device& device) {
  data_bus.register_device(device);
  _cpu->recieve(5);
}

void vm::add_device(device& dev) {
  data_bus.register_device(device);
}
