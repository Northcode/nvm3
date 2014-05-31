#pragma once
#include "head.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "media_device.hpp"
#include "text_screen.hpp"

class vm
{
public:
  std::shared_ptr<cpu> cpu1;
  std::shared_ptr<bus> data_bus;
  std::shared_ptr<ram> memory;

  vm();
  ~vm();

  void power();

  void reset();

  void insert_media(std::shared_ptr<media_device> media);

  void add_device(std::shared_ptr<device> dev);
};

vm::vm() {
  if (DEBUG_OUT)
    std::cout << "const vm " << this << std::endl;
  cpu1 = std::shared_ptr<cpu>(new cpu());
  data_bus = std::shared_ptr<bus>(new bus());
  memory = std::shared_ptr<ram>(new ram());
  cpu1->data_bus = data_bus;
  cpu1->memory = memory;
  cpu1->PG.memory = memory;
  data_bus->register_device(cpu1);
  data_bus->register_device(memory);
  auto textdev = std::shared_ptr<text_device>(new text_device());
  data_bus->register_device(textdev);
  memory->map_device(textdev);
}


vm::~vm() {
  data_bus->free_devices();
  if(DEBUG_OUT){
    std::cout << "cpu use count " << cpu1.use_count() << std::endl;
    std::cout << "mem use count " << memory.use_count() << std::endl;
    std::cout << "bus use count " << data_bus.use_count() << std::endl;
  }
  data_bus.reset();
  cpu1.reset();
  memory.reset();
  if(DEBUG_OUT)
    std::cout << "dest vm " << this << std::endl;
}


void vm::add_device(std::shared_ptr<device> dev) {
  data_bus->register_device(dev);
}

void vm::power() {
  cpu1->execute();
}

void vm::reset() {
  cpu1->reset();
  cpu1->execute();
}

void vm::insert_media(std::shared_ptr<media_device> dev) {
  data_bus->register_device(dev);
  cpu1->exec_interupt(NEW_HARDWARE);
}
