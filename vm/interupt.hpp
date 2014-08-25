#ifndef INTERUPTS_H
#define INTERUPTS_H

enum interupts
{
  div_by_zero,
  breakpoint,
  invalid_opcode,
  gp_fault,
  page_fault,
  stack_overflow,
  no_bios,
  double_fault,
  triple_fault,
  new_hardware,
  invalid_interupt
};
#endif
