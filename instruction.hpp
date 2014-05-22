#pragma once

struct instruction
{
  byte opcode;
  byte register;
  addr_mode addressing_mode;
  byte flags;
  int data;
};
