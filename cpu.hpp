#pragma once
#include "heap.hpp"
#include "bus.hpp"
#include "pager.hpp"
#include "instruction.hpp"

class cpu : device
{
public:
  //registers
  byte A,B,C,D;
  int AX,BX,CX,DX;
  long AXX,BXX,CXX,DXX;

  maddr IP,SP,BP,HP,SI,DI;

  bool EQ,GT;

  bool PE,MD;

  pager PG;

  void tick();

  void execute();

  instruction nextInstruction();
};
