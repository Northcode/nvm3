#pragma once
#include <iostream>
#include <vector>

typedef unsigned char byte;
typedef size_t maddr;

constexpr int RAM_SIZE = 4*4*1024;
constexpr int PAGE_DIRECTORY_SIZE = 1024;
constexpr int PAGE_TABLE_SIZE = 1024;
constexpr int PAGE_SIZE = 4*1024;

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
