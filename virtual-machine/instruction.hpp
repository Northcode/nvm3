#pragma once
#include "head.hpp"
#include "ram.hpp"

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

  void write(ram& outs, maddr address) {
    outs.write(address,     opcode);
    outs.write(address + 1, reg);
    outs.write(address + 2, (byte)addressing_mode);
    outs.write(address + 3, flags);
    outs.write(address + 4, (maddr)data);
  }

  void write(std::shared_ptr<ram> outs, maddr address) {
    if(DEBUG_OUT)
      std::cout << "writing instruction " << (char)(opcode + 48) << " to " << address << std::endl;
    outs->write(address,     opcode);
    outs->write(address + 1, reg);
    outs->write(address + 2, (byte)addressing_mode);
    outs->write(address + 3, flags);
    outs->write(address + 4, (maddr)data);
  }
};
