#pragma once

struct instruction
{
  byte opcode;
  byte reg;
  addr_mode addressing_mode;
  byte flags;
  int data;
};
