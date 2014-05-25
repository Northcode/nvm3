#pragma once
#include "head.hpp"
#include "device.hpp"

struct media_device : public device
{
  std::vector<byte> data;
};
