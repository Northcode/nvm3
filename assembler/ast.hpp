#include <string>
#include "../vm/instruction.hpp"

#ifndef AST_H
#define AST_H

/*

NORTHCODE VM ASSEMBLY ABSTRACT SYNTAX TREE

base tokens:
<word>,<number>,<string>,<symbol>,<char>

higher level ast nodes:

# BASIC DATA TYPES:

<label> := <word>:

<int> := <number>
          | -<number>

<value> := <int>
        |  <char>

<data_at_address> := [<address>]
                  |  IP+[<address>]
                  |  SP+[<address>]

<address> := <word>(from labels)
          |  <number>
          |  IP+<number>
          |  SP+<number>

<register> := <any>(A,B,C,D,AX,BX,CX,DX,IP,SP,BP,DA,DB,DC,DD,EQ,GT,PD,PT,PE,MD,RN)

<address_of_register> := [<register>]
                      |  IP+[<register>]
                      |  SP+[<register>]

#ARGUMENT TYPES:

<Larg> := <register>
        | <address_of_register>

<Jarg> := <Larg>
        | <address>
        | <data_at_address>

<Rarg> := <Larg>
        | <address>
        | <value>
        | <data_at_address>

#INSTRUCTIONS:

<mov> := mov <Larg>,<Rarg>
<jmp> := jmp <Jarg>
<call> := call <Jarg>
<ret> := ret
<add> := add <register>, <Rarg>
<sub> := sub <register>, <Rarg>
<mul> := mul <register>, <Rarg>
<div> := div <register>, <Rarg>
<mod> := mod <register>, <Rarg>
<pow> := pow <register>, <Rarg>
<neg> := neg <register>
<sqrt> := sqrt <register>
<cmp> := cmp <Rarg>,<Rarg> # max <value> for Rarg is 8-bits
<outb> := outb <number>, <Rarg>
<outdw> := outdw <number>, <Rarg>
<inb> := inb <number>, <register>
<indw> := indw <number>, <register>
<int_> := int <number>
<rint> := rint <number>,<address>
<iret> := iret
<pushb> := pushb <Rarg>
<pushdw> := pushdw <Rarg>
<popb> := popb <register>
<popdw> := popdw <register>

#DEFINE BYTES:

<ds> := db <string>
<di> := di <int>
<db> := db <char>
      | db <number>
*/

namespace ast
{

enum ast_type
{
  char_,
  word,
  number,
  register_,
  label,
  int_,
  IP_rel_reg,
  SP_rel_reg,
  IP_rel_num,
  SP_rel_num,
  addr_of_reg,
  Larg,
  data_at_addr,
  address,
  Rarg,
  Jarg
};

struct ast_node
{
  ast_type type;
};

struct instruction_
{

};

struct Rarg_a : public ast_node
{
};

struct Jarg_a : public ast_node
{
};

struct Larg_a : public Rarg_a, public Jarg_a
{
};

struct RLins : public instruction_
{
  std::shared_ptr<Rarg_a> arg1;
  std::shared_ptr<Larg_a> arg2;
};

struct Lins : public instruction_
{
  std::shared_ptr<Larg_a> arg;
};

struct Rins : public instruction_
{
  std::shared_ptr<Rarg_a> arg;
};

struct Jins : public instruction_
{
  std::shared_ptr<Jarg_a> arg;
};

struct Nins : public instruction_
{
};

struct NRins : public instruction_
{
  std::shared_ptr<dword> arg1;
  std::shared_ptr<Rarg_a> arg2;
};

struct NLins : public instruction_
{
  std::shared_ptr<dword> arg1;
  std::shared_ptr<Larg_a> arg2;
};

struct mov : public RLins
{

};

struct jmp : public instruction_
{

};

struct call : public instruction_
{

};

struct ret : public instruction_
{

};

struct _int : public instruction_
{

};

struct iret : public instruction_
{

};

struct rint : public instruction_
{

};

struct add : public RLins
{

};

struct sub : public RLins
{

};

struct mul : public RLins
{

};

struct div : public RLins
{

};

struct mod : public RLins
{

};

struct neg : public instruction_
{

};

struct sqrt_ : public instruction_
{

};

struct cmp : public RLins
{

};

struct outb : public NLins
{

};

struct outdw : public NLins
{

};

struct inb : public NRins
{

};

struct indw : public NRins
{

};

struct pushb : public Rins
{

};

struct pushdw : public Rins
{

};

struct popb : public Lins
{

};

struct popdw : public Lins
{

};

struct data_at_address : public Rarg_a, public Jarg_a
{

};

struct address_a : public data_at_address, public Rarg_a, public Jarg_a
{

};

struct address_of_register : public Larg_a
{

};

struct IP_rel_reg_a : public address_of_register
{

};

struct SP_rel_reg_a : public address_of_register
{

};

struct IP_rel_num_a : public data_at_address
{

};

struct SP_rel_num_a : public data_at_address
{

};

struct value : public Rarg_a
{

};

struct label_a : public ast_node
{

};

struct int_a : public value
{
  bool is_neg;
};

struct number_a : public IP_rel_num_a, public SP_rel_num_a, public int_a
{
  size_t num;
};

struct register_a : public Larg_a, public IP_rel_reg_a, public SP_rel_reg_a
{
  register_e reg;
};

struct word_a : public address_a, public label_a, public register_a
{
  std::string word;
};

struct char_a : public value
{
  char ch;
};

struct NJins : public instruction
{
  std::shared_ptr<number_a> arg1;
  std::shared_ptr<Jarg_a> arg2;
};

};
#endif
