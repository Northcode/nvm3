#pragma once
#include <string>
#include "../virtual-machine/head.hpp"

constexpr byte T_INT{1};
constexpr byte T_STRING{2};
constexpr byte T_WORD{3};
constexpr byte T_SYM{4};

struct token
{
  byte token_type{};

  virtual ~token() {}
};

struct token_i : public token
{
  int data;
};

struct token_s : public token
{
  std::string data;
};

struct token_c : public token
{
  char data;
};

class assembler
{
public:
  std::vector<std::unique_ptr<token>> tokens;


};
