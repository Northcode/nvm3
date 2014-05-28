#pragma once

struct instruction
{
  byte opcode;
  byte reg;
  addr_mode addressing_mode;
  byte flags;
  int data;

  instruction(byte opcode, byte reg, addr_mode addressing_mode, byte flags, int data) {
    this->opcode           = opcode;
    this->reg              = reg;
    this->addressing_mode  = addressing_mode;
    this->flags            = flags;
    this->data             = data;
  }
};
