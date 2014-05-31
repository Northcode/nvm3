#pragma once
#include <iostream>
#include <vector>
#include <memory>

typedef unsigned char byte;
typedef unsigned int maddr;

#include "opcodes.hpp"

bool DEBUG_OUT{false};

constexpr std::size_t RAM_SIZE{4*4*1024};
constexpr std::size_t PAGE_DIRECTORY_SIZE{1024};
constexpr std::size_t PAGE_TABLE_SIZE{1024};
constexpr std::size_t PAGE_SIZE{4*1024};

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

void gotoxy(int x,int y)
{
  printf("%c[%d;%df",0x1B,y,x);
}
