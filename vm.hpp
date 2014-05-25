#pragma once
#include "head.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "media_device.hpp"

class vm
{
  bus data_bus{};

  std::shared_ptr<cpu> _cpu;

public:
  vm();

  void power();

  void reset();

  void insert_media(std::shared_ptr<media_device> media);

  void add_device(std::shared_ptr<device> dev);
};

vm::vm() {
  _cpu = std::shared_ptr<cpu>(new cpu());
  data_bus.register_device(_cpu);
}

void vm::add_device(std::shared_ptr<device> dev) {
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

void vm::insert_media(std::shared_ptr<media_device> dev) {
  data_bus.register_device(dev);
  _cpu->exec_interupt(8);
}
