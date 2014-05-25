#pragma once
#include "head.hpp"
#include "bus.hpp"
#include "pager.hpp"
#include "instruction.hpp"

class cpu : public device
{
public:
  //registers
  byte A{0},B{0},C{0},D{0};
  int AX{0},BX{0},CX{0},DX{0};
  long AXX{0},BXX{0},CXX{0},DXX{0};

  maddr IP{0},SP{0},BP{0},HP{0},SI{0},DI{0};

  maddr IVT{0};

  bool EQ{false},GT{false},RN{false},HLT{false};

  bool PE{false},MD{false};

  pager PG{};

  void tick();

  void execute();

  void reset();

  void halt();

  instruction nextInstruction();

  void exec_interupt(int i);

  void mov(instruction ins);

  void jump(instruction ins);

  void cmp(instruction ins);

  void math(instruction ins);

  void interupt(instruction ins);

  void busio(instruction ins);

  //device implementation
  void recieve(byte data);
  void recieve(maddr data);

  byte send_byte();
  maddr send_int();

  bool bus_width();
};

void cpu::tick() {
  if(HLT) {
    exec_interupt(0);
  }
  instruction i = nextInstruction();
  if(i.opcode == 0) {
    mov(i);
  } else if (i.opcode == 1) {
    jump(i);
  } else if (i.opcode == 2) {
    cmp(i);
  } else if (i.opcode == 3) {
    math(i);
  } else if (i.opcode == 4) {
    interupt(i);
  } else if (i.opcode == 5) {
    busio(i);
  }
  IP += 8;
}

void cpu::execute() {
  RN = true;
  HLT = false;
  while(RN) {
    tick();
  }
}

void cpu::halt() {
  HLT = true;
}

void cpu::recieve(maddr data) {

}
