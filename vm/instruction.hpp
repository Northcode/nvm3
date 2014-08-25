#include "main.hpp"


#ifndef INSTRUCTION_H
#define INSTRUCTION_H
// Instruction:
// 1 byte - opcode
// 1 byte - addressing_mode
// 1 byte - arg types
// 1 byte - register
// 4 byte - data

enum opcode // 1 byte
{
  nop,
  mov,
  jmp,
  math,
  interupt,
  io,
  stack,
  cmp
};

enum register_e // 1 byte
{
  A = 0,B,C,D,
  AX,BX,CX,DX,
  IP,SP,BP,
  DA,DB,DC,DD,
  EQ,GT,
  PD,PT,PE,MD,
  RN
};

register_e get_register(dword index) {
  return (register_e)index;
}

union raw_instruction {
  struct
  {
    byte op:8;
    byte addr_mode_a:4;
    byte addr_mode_b:4;
    byte arg_type_a:4;
    byte arg_type_b:4;
    byte reg:8;
  };
  dword raw;
};

enum addressing_mode // 2 bit
{
  direct,
  IP_relative,
  SP_relative
};

enum argument_type // 4 bit
{
  registers,
  data_at_address,
  address_of_register,
  value,
};

union mov_i {
  struct
  {
    byte op:8;
    addressing_mode addr_mode_a:4;
    addressing_mode addr_mode_b:4;
    argument_type type_a:4;
    argument_type type_b:4;
    register_e reg:8;
    dword data:32;
  };
  long raw;
};

union jmp_i {
  struct
  {
    byte op:8;
    addressing_mode addr_mode:4;
    argument_type type:4;
    byte flag:8;
    register_e reg:8;
    dword data:32;
  };
  long raw;
};

union math_i {
  struct
  {
    byte op:8;
    addressing_mode addr_mode:4;
    argument_type type:4;
    byte flag:8;
    register_e reg:8;
    dword data:32;
  };
  long raw;
};

union interupt_i {
  struct
  {
    byte op:8;
    byte flag:8;
    byte index:8;
    addressing_mode addr_mode:8;
    dword address:32;
  };
  long raw;
};

union io_i {
  struct
  {
    byte op:8;
    addressing_mode addr_mode:4;
    argument_type type:4;
    byte flag:8;
    byte port:8;
    dword data:32;
  };
  long raw;
};

union stack_i {
  struct
  {
    byte op:8;
    addressing_mode addr_mode:4;
    argument_type type:4;
    byte flag:8;
    register_e reg:8;
    dword data:32;
  };
  long raw;
};

union cmp_i {
  struct
  {
    byte op:8;
    addressing_mode addr_mode_a:4;
    argument_type type_a:4;
    dword val_a:8;
    addressing_mode addr_mode_b:4;
    argument_type type_b:4;
    dword val_b:32;
  };
  long raw;
};

struct instruction
{
  opcode op;
  addressing_mode addr_mode_a,addr_mode_b;
};

struct mov_arg
{
  addressing_mode addr_mode;
  argument_type type;
  dword data;
};

struct mov_instruction : public instruction
{
  mov_arg dest,source;
};

struct jmp_instruction : public instruction
{
  mov_arg address;
};

enum math_op
{
  add, subtract, multiply, divide, modulo
};

struct math_instruction : public instruction
{
  math_op op;
  dword A;
  mov_arg B;
};

struct interupt_instruction : public instruction
{
  dword interupt;
};

struct io_instruction : public instruction
{
  bool is_input;
  dword port;
};

struct stack_instruction : public instruction
{
  bool is_pop;
  mov_arg arg;
};
#endif
