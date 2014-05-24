#pragma once
#include "head.hpp"
#include "bus.hpp"
#include "pager.hpp"
#include "instruction.hpp"

class cpu : device
{
public:
  //registers
  byte A{0},B{0},C{0},D{0};
  int AX{0},BX{0},CX{0},DX{0};
  long AXX{0},BXX{0},CXX{0},DXX{0};

  maddr IP{0},SP{0},BP{0},HP{0},SI{0},DI{0};

  bool EQ{0},GT{0};

  bool PE{0},MD{0};

  pager PG{};

  void tick();

  void execute();

  instruction nextInstruction();

  void mov(instruction ins);

  void jump(instruction ins);

  void cmp(instruction ins);

  void math(instruction ins);

  void interupt(instruction ins);

  void busio(instruction ins);
};
