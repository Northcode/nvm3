#pragma once
#include "head.hpp"
#include "ram.hpp"
#include "bus.hpp"
#include "pager.hpp"
#include "instruction.hpp"
#include "exceptions.hpp"

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

  bool PE{false},MD{false},TRAP{false},FAULT{false};

  maddr FAULT_ADDR{0};

  pager PG{};

  int get_register_size(int reg);

  void set_register(int reg, byte data);

  void set_register(int reg, int data);

  void set_register(int reg, long data);

  byte get_register_b(int reg);

  int get_register_i(int reg);

  long get_register_l(int reg);

  void write(maddr address, byte data);

  void write(maddr address, maddr data);

  byte read(maddr address);

  maddr read_int(maddr address);

  void push(byte data);

  void push(maddr data);

  byte pop();

  maddr pop_dw();

  void tick();

  void execute();

  void reset();

  void halt();

  instruction next_instruction();

  void exec_interupt(int i);

  void exec_hw_interupt(int i);

  void ret_interupt();

  void force_trap(int trap);

  void jmp(instruction ins);

  void mov(instruction ins);

  void jump(instruction ins);

  void ccmp(instruction ins);

  void cmp(instruction ins);

  void math(instruction ins);

  void interupt(instruction ins);

  void busio(instruction ins);

  void stackop(instruction ins);

  //device implementation
  void recieve(maddr data);

  maddr send_int();

  bool bus_width();
};

cpu cpu1{};

int cpu::get_register_size(int reg) {
  if (reg < 4)
    return 8;
  else if (reg < 8)
    return 32;
  else if (reg < 12)
    return 64;
  else if (reg < 20)
    return 32;
  else if (reg == 28)
    return 32;
  else
    return 1;
}

void cpu::set_register(int reg, byte data){
  if(reg == 0)
    A = data;
  else if (reg == 1)
    B = data;
  else if (reg == 2)
    C = data;
  else if (reg == 3)
    D = data;
  else if (reg == 20)
    EQ = data == 1;
  else if (reg == 21)
    GT = data == 1;
  else if (reg == 22)
    if(!MD)
      force_trap(GP_FAULT);
    else
    RN = data == 1;
  else if (reg == 23)
    if(!MD)
      force_trap(GP_FAULT);
    else
    HLT = data == 1;
  else if (reg == 24)
    if(!MD)
      force_trap(GP_FAULT);
    else
    PE = data == 1;
  else if (reg == 25)
    if(!MD)
      force_trap(GP_FAULT);
    else
    MD = data == 1;
  else if (reg == 26)
    if(!MD)
      force_trap(GP_FAULT);
    else
    TRAP = data == 1;
  else if (reg == 27)
    if(!MD)
      force_trap(GP_FAULT);
    else
    FAULT = data == 1;
}

void cpu::set_register(int reg, int data){
  if (reg == 4)
    AX = data;
  else if (reg == 5)
    BX = data;
  else if (reg == 6)
    CX = data;
  else if (reg == 7)
    DX = data;
  else if (reg == 12)
    if(!MD)
      force_trap(GP_FAULT);
    else
    IP = data;
  else if (reg == 13)
    SP = data;
  else if (reg == 14)
    BP = data;
  else if (reg == 15)
    HP = data;
  else if (reg == 16)
    SI = data;
  else if (reg == 17)
    DI = data;
  else if (reg == 18)
    if(!MD)
      force_trap(GP_FAULT);
    else
    IVT = data;
  else if (reg == 19)
    if(!MD)
      force_trap(GP_FAULT);
    else
      PG.PT = data;
  else if (reg == 28)
    if(!MD)
      force_trap(GP_FAULT);
    else
      FAULT_ADDR = data;
}

void cpu::set_register(int reg, long data){
  if (reg == 8)
    AXX = data;
  else if (reg == 9)
    BXX = data;
  else if (reg == 10)
    CXX = data;
  else if (reg == 11)
    DXX = data;
}

byte cpu::get_register_b(int reg){
  if(reg == 0)
    return A;
  else if (reg == 1)
    return B;
  else if (reg == 2)
    return C;
  else if (reg == 3)
    return D;
  else if (reg == 20)
    return EQ ? 1 : 0;
  else if (reg == 21)
    return GT ? 1 : 0;
  else if (reg == 22)
    if(!MD)
      force_trap(GP_FAULT);
    else
    return RN ? 1 : 0;
  else if (reg == 23)
    if(!MD)
      force_trap(GP_FAULT);
    else
    return HLT ? 1 : 0;
  else if (reg == 24)
    if(!MD)
      force_trap(GP_FAULT);
    else
    return PE ? 1 : 0;
  else if (reg == 25)
    if(!MD)
      force_trap(GP_FAULT);
    else
    return MD ? 1 : 0;
  else if (reg == 26)
    if(!MD)
      force_trap(GP_FAULT);
    else
    return TRAP ? 1 : 0;
  else if (reg == 27)
    if(!MD)
      force_trap(GP_FAULT);
    else
    return FAULT ? 1 : 0;
  return 0;
}

int cpu::get_register_i(int reg){
  if (reg == 4)
    return AX;
  else if (reg == 5)
    return BX;
  else if (reg == 6)
    return CX;
  else if (reg == 7)
    return DX;
  else if (reg == 12)
    if(!MD)
      force_trap(GP_FAULT);
    else
      return IP;
  else if (reg == 13)
    return SP;
  else if (reg == 14)
    return BP;
  else if (reg == 15)
    return HP;
  else if (reg == 16)
    return SI;
  else if (reg == 17)
    return DI;
  else if (reg == 18)
    if(!MD)
      force_trap(GP_FAULT);
    else
      return IVT;
  else if (reg == 19)
    if(!MD)
      force_trap(GP_FAULT);
    else
      return PG.PT;
  else if (reg == 28)
    if(!MD)
      force_trap(GP_FAULT);
    else
      return FAULT_ADDR;
  return 0;
}

long cpu::get_register_l(int reg){
  if (reg == 8)
    return AXX;
  else if (reg == 9)
    return BXX;
  else if (reg == 10)
    return CXX;
  else if (reg == 11)
    return DXX;
  else
    return 0;
}

void cpu::write(maddr address, byte data){
  if(PG.can_write(address))
    memory.write(address,data);
  else {
    FAULT_ADDR = address;
    force_trap(PAGE_FAULT);
  }
}

void cpu::write(maddr address, maddr data){
  if(PG.can_write(address))
    memory.write(address,data);
  else {
    FAULT_ADDR = address;
    force_trap(PAGE_FAULT);
  }
}

byte cpu::read(maddr address){
  if(PG.can_read(address))
    return memory.read(address);
  else {
    FAULT_ADDR = address;
    force_trap(PAGE_FAULT);
  }
  return 0;
}

maddr cpu::read_int(maddr address) {
  if(PG.can_read(address))
    return memory.read_int(address);
  else {
    FAULT_ADDR = address;
    force_trap(PAGE_FAULT);
  }
  return 0;
}

void cpu::push(byte data) {
  memory.write(SP,data);
  SP++;
}

void cpu::push(maddr data) {
  memory.write(SP,data);
  SP += 4;
}

byte cpu::pop() {
  SP--;
  return memory.read(SP);
}

maddr cpu::pop_dw() {
  SP -= 4;
  return memory.read_int(SP);
}

void cpu::tick() {
  if(HLT) {
    return;
  }
  instruction i = next_instruction();

  if (i.flags >= 128) {
    force_trap(DEBUG_BREAKPOINT);
    return;
  }

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

void cpu::reset() {
  IP = 0;
}

void cpu::halt() {
  HLT = true;
}

instruction cpu::next_instruction() {
  byte opcode       = read(IP);
  byte reg          = memory.read(IP + 1);
  byte addr_mode_b  = memory.read(IP + 2);
  byte flags        = memory.read(IP + 3);
  int data          = memory.read_int(IP + 4);
  instruction ins{opcode,reg,static_cast<addr_mode>(addr_mode_b),flags,data};
  return ins;
}

void cpu::exec_interupt(int i) {
  maddr address = memory.read_int(IVT + i * 4);
  if(address == 0) {
    force_trap(DOUBLE_FAULT);
  }
  push(IP);
  push((byte)(MD ? 1 : 0));
  MD = true;
  IP = address;
}

void cpu::exec_hw_interupt(int i) {

}

void cpu::ret_interupt() {
  MD = (pop() == 1);
  IP = pop_dw();
}

void cpu::force_trap(int trap) {
  if(FAULT)
    exec_interupt(TRIPLE_FAULT);
  if(TRAP) {
    FAULT = true;
    exec_interupt(DOUBLE_FAULT);
  } else {
    TRAP = true;
    exec_interupt(trap);
  }
}

void cpu::jmp(instruction ins) {
  if(ins.addressing_mode == reg_reg)
    IP += get_register_i(ins.reg);
  else if (ins.addressing_mode == reg_val)
    IP = ins.data;
  else if (ins.addressing_mode == reg_adr)
    IP = read_int(ins.data);
}

void cpu::mov(instruction ins) {
  int register_a_size = get_register_size(ins.reg);
  if(ins.addressing_mode == reg_reg) {
    int register_b_size = get_register_size(ins.data);
    if(register_a_size != register_b_size) {
      force_trap(GP_FAULT);
      return;
    }
    if(register_b_size <= 8)
      set_register(ins.reg, get_register_b(ins.data));
    else if (register_b_size == 32)
      set_register(ins.reg, get_register_i(ins.data));
    else if (register_b_size == 64)
      set_register(ins.reg, get_register_l(ins.data));
  } else if (ins.addressing_mode == reg_val) {
    if(register_a_size <= 8)
      set_register(ins.reg, static_cast<byte>(ins.data));
    else if (register_a_size == 32)
      set_register(ins.reg, ins.data);
    else if (register_a_size == 64)
      set_register(ins.reg, static_cast<long>(ins.data));
  } else if (ins.addressing_mode == reg_adr) {
    if(register_a_size == 8)
      set_register(ins.reg, read(ins.data));
    else
      set_register(ins.reg, static_cast<int>(read_int(ins.data)));
  } else if (ins.addressing_mode == reg_aor) {
    if(register_a_size == 8)
      set_register(ins.reg, read(get_register_b(ins.data)));
    else
      set_register(ins.reg, static_cast<int>(read_int(get_register_i(ins.data))));
  } else if (ins.addressing_mode == aor_reg) {
    int register_b_size = get_register_size(ins.data);
    if(register_a_size == 8) {
      if(register_b_size == 8)
        write(get_register_b(ins.reg),get_register_b(ins.data));
      else
        write(get_register_b(ins.reg), (maddr)get_register_i(ins.data));
    } else {
      if(register_b_size == 8)
        write(get_register_i(ins.reg),get_register_b(ins.data));
      else
        write(get_register_i(ins.reg), (maddr)get_register_i(ins.data));
    }
  } else if (ins.addressing_mode == aor_aor) {
    int register_b_size = get_register_size(ins.data);
    if(register_a_size == 8) {
      if(register_b_size == 8)
        write(get_register_b(ins.reg), read(get_register_b(ins.data)));
      else
        write(get_register_b(ins.reg), read_int(get_register_i(ins.data)));
    } else {
      if(register_b_size == 8)
        write(get_register_i(ins.reg), read(get_register_b(ins.data)));
      else
        write(get_register_i(ins.reg), read_int(get_register_i(ins.data)));
    }
  } else if (ins.addressing_mode == adr_reg) {
    if(!PG.can_write(ins.data))
      force_trap(PAGE_FAULT);
    if(register_a_size == 8)
      write(ins.data, get_register_b(ins.reg));
    else
      write(ins.data, (maddr)get_register_i(ins.reg));
  }
}

void cpu::jump(instruction ins) {
  if(ins.flags == 0)
  { jmp(ins); }
  else if(ins.flags == 1)
  { if(EQ){ jmp(ins); } }
  else if(ins.flags == 2)
  { if(!EQ){ jmp(ins); } }
  else if(ins.flags == 3)
  { if(!EQ && !GT){ jmp(ins); } }
  else if(ins.flags == 4)
  { if(EQ && !GT){ jmp(ins); } }
  else if(ins.flags == 5)
  { if(GT){ jmp(ins); } }
  else if(ins.flags == 6)
  { if(EQ && GT){ jmp(ins); } }
}

void cpu::ccmp(instruction ins) {
  if(ins.addressing_mode == reg_reg) {
    int reg_a_size = get_register_size(ins.reg);
    int reg_b_size = get_register_size(ins.data);
    if(reg_a_size <= 8) {
      byte a = get_register_b(ins.reg);
      if(reg_b_size <= 8) {
        byte b = get_register_b(ins.data);
        EQ = a == b;
        GT = a > b;
      } else if (reg_b_size == 32) {
        int b = get_register_i(ins.data);
        EQ = a == b;
        GT = a > b;
      } else if (reg_b_size == 64) {
        long b = get_register_i(ins.data);
        EQ = a == b;
        GT = a > b;
      }
    } else if (reg_a_size == 32) {
      int a = get_register_b(ins.reg);
      if(reg_b_size == 8) {
        byte b = get_register_b(ins.data);
        EQ = a == b;
        GT = a > b;
      } else if (reg_b_size == 32) {
        int b = get_register_i(ins.data);
        EQ = a == b;
        GT = a > b;
      } else if (reg_b_size == 64) {
        long b = get_register_i(ins.data);
        EQ = a == b;
        GT = a > b;
      }
    } else if (reg_a_size == 64) {
      long a = get_register_b(ins.reg);
      if(reg_b_size == 8) {
        byte b = get_register_b(ins.data);
        EQ = a == b;
        GT = a > b;
      } else if (reg_b_size == 32) {
        int b = get_register_i(ins.data);
        EQ = a == b;
        GT = a > b;
      } else if (reg_b_size == 64) {
        long b = get_register_i(ins.data);
        EQ = a == b;
        GT = a > b;
      }
    }
  } else if (ins.addressing_mode == reg_val) {
    int reg_a_size = get_register_size(ins.reg);
    if(reg_a_size <= 8) {
      byte a = get_register_b(ins.reg);
      EQ = a == ins.data;
      GT = a > ins.data;
    } else if (reg_a_size == 32) {
      int a = get_register_i(ins.reg);
      EQ = a == ins.data;
      GT = a > ins.data;
    } else {
      long a = get_register_l(ins.reg);
      EQ = a == ins.data;
      GT = a > ins.data;
    }
  }
}

void cpu::cmp(instruction ins) {
  ccmp(ins);
}

void cpu::math(instruction ins) {
  if(ins.flags >= 4) {
    int reg_size{get_register_size(ins.reg)};

    if(reg_size == 8) {
      if(ins.flags == 4)
        return;
      else if (ins.flags == 5)
        set_register(ins.reg, get_register_b(ins.reg) + 1);
      else
        set_register(ins.reg, get_register_b(ins.reg) - 1);
    } else if(reg_size == 32) {
      if(ins.flags == 4)
        set_register(ins.reg, -get_register_i(ins.reg));
      else if (ins.flags == 5)
        set_register(ins.reg, get_register_i(ins.reg) + 1);
      else
        set_register(ins.reg, get_register_i(ins.reg) - 1);
    } else if(reg_size == 64) {
      if(ins.flags == 4)
        set_register(ins.reg, -get_register_l(ins.reg));
      else if (ins.flags == 5)
        set_register(ins.reg, get_register_l(ins.reg) + 1);
      else
        set_register(ins.reg, get_register_l(ins.reg) - 1);
    }
  } else if(ins.addressing_mode == reg_reg) {
    int reg_a_size{get_register_size(ins.reg)};
    int reg_b_size{get_register_size(ins.data)};
    int reg_a{0};
    int reg_b{0};
    if(reg_a_size <= 8)
      reg_a = get_register_b(ins.reg);
    else if(reg_a_size == 32)
      reg_a = get_register_i(ins.reg);
    else
      reg_a = get_register_l(ins.reg);

    if(reg_b_size <= 8)
      reg_b = get_register_b(ins.data);
    else if(reg_b_size == 32)
      reg_b = get_register_i(ins.data);
    else
      reg_b = get_register_l(ins.data);

    int result{0};

    if(ins.flags == 0)
      result = reg_a + reg_b;
    else if(ins.flags == 1)
      result = reg_a - reg_b;
    else if(ins.flags == 2)
      result = reg_a * reg_b;
    else
      if(reg_b == 0) {
        force_trap(DIV_BY_ZERO);
        return;
      } else
        result = reg_a / reg_b;

    if(reg_a_size <= 8)
      set_register(ins.reg, (byte)(result));
    else if(reg_a_size == 32)
      set_register(ins.reg, (int)(result));
    else
      set_register(ins.reg, (long)(result));
  } else if (ins.addressing_mode == reg_val) {
    int reg_a_size{get_register_size(ins.reg)};

    int reg_a{0};

    if(reg_a_size <= 8)
      reg_a = get_register_b(ins.reg);
    else if (reg_a_size == 32)
      reg_a = get_register_i(ins.reg);
    else if (reg_a_size = 64)
      reg_a = get_register_l(ins.reg);

    int result{0};

    if(ins.flags == 0)
      result = reg_a + ins.data;
    else if (ins.flags == 1)
      result = reg_a - ins.data;
    else if (ins.flags == 2)
      result = reg_a * ins.data;
    else if (ins.flags == 3)
      if(ins.data == 0)
        force_trap(DIV_BY_ZERO);
      else
        result = reg_a / ins.data;

    if(reg_a_size <= 8)
      set_register(ins.reg, (byte)(result));
    else if(reg_a_size == 32)
      set_register(ins.reg, (int)(result));
    else
      set_register(ins.reg, (long)(result));
  }
}

void cpu::interupt(instruction ins) {
  if(ins.flags == 0) {
    exec_hw_interupt(ins.data);
  } else if (ins.flags == 1) {
    exec_interupt(ins.data);
  } else if (ins.flags == 2) {
    int reg_size{get_register_size(ins.reg)};
    if(reg_size != 32)
      return;

    write(IVT + ins.data * 4, (maddr)get_register_i(ins.reg));
  } else if (ins.flags == 3) {
    ret_interupt();
  }
}

void cpu::busio(instruction ins) {
  if(ins.flags == 0) {
    int reg_size{get_register_size(ins.reg)};

    if(reg_size == 8)
      data_bus.out(ins.data, get_register_b(ins.reg));
    else if(reg_size == 32)
      data_bus.out(ins.data, (maddr)get_register_i(ins.reg));
  } else if (ins.flags == 1) {
    int reg_size{get_register_size(ins.reg)};

    if(reg_size == 8)
      set_register(ins.reg, data_bus.inb(ins.data));
    else if (reg_size == 32)
      set_register(ins.reg, (int)data_bus.indw(ins.data));
  }
}

void cpu::stackop(instruction ins) {
  if(ins.flags == 0) {
    int reg_size{get_register_size(ins.reg)};

    if(reg_size <= 8)
      push(get_register_b(ins.reg));
    else if(reg_size == 32)
      push((maddr)get_register_i(ins.reg));
  } else if (ins.flags == 1) {
    push((byte)ins.data);
  } else if (ins.flags == 2) {
    push((maddr)ins.data);
  } else if (ins.flags == 3) {
    int reg_size{get_register_size(ins.reg)};

    if(reg_size <= 8)
      set_register(ins.reg, pop());
    else if (reg_size == 32)
      set_register(ins.reg, (int)pop_dw());
  }
}

void cpu::recieve(maddr data) {

}

maddr cpu::send_int() {
  return 0;
}

bool cpu::bus_width() { return true; }
