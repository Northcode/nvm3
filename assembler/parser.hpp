#include "lexer.hpp"
#include "../vm/instruction.hpp"
#include "ast.hpp"

#ifndef PARSER_H
#define PARSER_H

struct parse_exception
{
  size_t tokenindex;
  std::string msg;

  parse_exception(size_t,std::string msg) : msg(msg) {}
};

template<typename T> using ptr = std::shared_ptr<T>;

struct parser
{
  std::vector<token> tokens;
  std::vector<ast::ast_node> nodes;
  size_t index;

  parser();

  token currenttoken();
  void next();

  bool is_word();
  bool is_number();
  bool is_string();
  bool is_symbol();
  bool is_char();

  void parse_all();

  ptr<ast::register_a> parse_register();
  ptr<ast::number_a> parse_number();
  ptr<ast::word_a> parse_word();
  ptr<ast::char_a> parse_char();

  ptr<ast::int_a> parse_int();
  ptr<ast::label_a> parse_label();
  ptr<ast::IP_rel_reg_a> parse_IP_rel_reg();
  ptr<ast::SP_rel_reg_a> parse_SP_rel_reg();
  ptr<ast::IP_rel_num_a> parse_IP_rel_num();
  ptr<ast::SP_rel_num_a> parse_SP_rel_num();

  ptr<ast::address_of_register> parse_addr_of_reg();
  ptr<ast::data_at_address> parse_data_at_addr();

  ptr<ast::address_a> parse_address();
  ptr<ast::value> parse_value();

  ptr<ast::Larg_a> parse_Larg();

  ptr<ast::Rarg_a> parse_Rarg();
  ptr<ast::Jarg_a> parse_Jarg();

  ptr<ast::mov> parse_mov();
  ptr<ast::jmp> parse_jmp();
  ptr<ast::call> parse_call();
  ptr<ast::ret> parse_ret();
  ptr<ast::_int> parse_interupt();
  ptr<ast::iret> parse_iret();
  ptr<ast::rint> parse_rint();
  ptr<ast::add> parse_add();
  ptr<ast::sub> parse_sub();
  ptr<ast::mul> parse_mul();
  ptr<ast::div> parse_div();
  ptr<ast::mod> parse_mod();
  ptr<ast::neg> parse_neg();
  ptr<ast::sqrt_> parse_sqrt();
  ptr<ast::cmp> parse_cmp();
  ptr<ast::outb> parse_outb();
  ptr<ast::outdw> parse_outdw();
  ptr<ast::inb> parse_inb();
  ptr<ast::indw> parse_indw();
  ptr<ast::pushb> parse_pushb();
  ptr<ast::pushdw> parse_pushdw();
  ptr<ast::popb> parse_popb();
  ptr<ast::popdw> parse_popdw();

  ptr<ast::instruction_> parse_instruction();

  ptr<ast::ast_node> parse_node();
};

#endif
