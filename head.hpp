#pragma once
#include <iostream>
#include <vector>
#include <memory>

typedef unsigned char byte;
typedef std::size_t maddr;

constexpr std::size_t RAM_SIZE = 4*4*1024;
constexpr std::size_t PAGE_DIRECTORY_SIZE = 1024;
constexpr std::size_t PAGE_TABLE_SIZE = 1024;
constexpr std::size_t PAGE_SIZE = 4*1024;

enum addr_mode
{
  reg_reg,
  reg_val,
  reg_adr,
  reg_aor,
  aor_reg,
  aor_aor,
  adr_reg
};
