#pragma once
#include "head.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "media_device.hpp"

class vm
{
public:
  vm();

  void power();

  void reset();

  void insert_media(std::shared_ptr<media_device> media);

  void add_device(std::shared_ptr<device> dev);
};

vm::vm() {
  data_bus.register_device(std::shared_ptr<device>(&cpu1));
  data_bus.register_device(std::shared_ptr<device>(&memory));
}

void vm::add_device(std::shared_ptr<device> dev) {
  data_bus.register_device(dev);
}

void vm::power() {
  cpu1.execute();
}

void vm::reset() {
  cpu1.halt();
  cpu1.reset();
  cpu1.execute();
}

void vm::insert_media(std::shared_ptr<media_device> dev) {
  data_bus.register_device(dev);
  cpu1.exec_interupt(8);
}
