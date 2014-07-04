#pragma once
#include "../virtual-machine/head.hpp"
#include <string>
#include <sstream>
#include <fstream>

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
  std::ifstream input_file;
  std::vector<std::unique_ptr<token>> tokens;

  assembler(std::string);

  void scan();

  void scan_char(char);
};

assembler::assembler(std::string filename) {
  input_file.open(filename, std::ifstream::in);
  tokens = std::vector<std::unique_ptr<token>>();
}

void assembler::scan() {
  while(input_file.good())
    scan_char(input_file.get());
}

void assembler::scan_char(char c) {
  if(c == '"') {

  }
}
