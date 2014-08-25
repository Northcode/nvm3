#include "main.hpp"

#ifndef CPU_H
#define CPU_H

struct cpu : public device
{
  cpu();

  void tick();
  void power();
  void reset();

  void exec_instruction(dword address);

  void mov(mov_i i);
  void jmp(jmp_i i);
  void math(math_i i);
  void interupt(interupt_i i);
  void stack(stack_i i);
  void io(io_i i);
  void cmp(cmp_i i);

  void jump(dword address);

  dword get_Rarg_val(dword data, argument_type type, addressing_mode mode);
};

#endif
