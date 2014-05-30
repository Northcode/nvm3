#pragma once
#include "head.hpp"
#include "device.hpp"

struct media_device : public device
{
  std::vector<byte> data;
public:
  media_device(std::vector<byte>&& data) : device() {
    this->data = data;
  }

  void recieve(maddr data) {

  }

  maddr send_int() {
    return 0;
  }

  bool bus_width() {
    return true;
  }
};
