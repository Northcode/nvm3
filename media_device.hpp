#pragma once
#include "head.hpp"
#include "device.hpp"

struct media_device : device
{
  std::vector<byte> data;
};
