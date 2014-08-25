#include <string>
#include <memory>
#include <vector>

#ifndef LEXER_H
#define LEXER_H

enum tokentype {
  word,
  number,
  string,
  character,
  symbol
};

struct token {
  tokentype type;
  std::shared_ptr<void> value;

  size_t l,c;

  tokentype get_type();

  std::string get_word();
  size_t get_number();
  std::string get_string();
  char get_symbol();
  char get_char();
};

struct lexer {
  size_t index;
  std::string code;
  std::vector<token> tokens;
  size_t line,col;

  lexer();

  char currentchar();
  char charat(size_t);
  void next();

  void scan_all();

  void scan_word();
  void scan_number();
  void scan_string();
  void scan_char();
  void scan_symbol();

  bool is_digit();
  bool is_char();
  bool is_quote();
  bool is_squote();
  bool is_newline();
  bool is_escape();
  bool is_eof();
  bool is_space();
  bool is_minus();
};



#endif
