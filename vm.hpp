#pragma once
#include "head.hpp"
#include "cpu.hpp"
#include "bus.hpp"
#include "media_device.hpp"

class vm
{
  bus data_bus{};

public:
  vm();

  void mount(std::vector<byte>& data, int index);

  void unmount(int index);

  void power();

  void reset();

  void insert_media(media_device& media);
};
