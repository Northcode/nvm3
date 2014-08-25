#include "lexer.hpp"
#include <cctype>
#include <sstream>
#include <cstdlib>
#include <iostream>

#ifndef LEXER_C
#define LEXER_C

tokentype token::get_type() {
  return type;
}

std::string token::get_word() {
  if(type == tokentype::word || type == tokentype::string) {
    return *((std::string*)value.get());
  } else {
    return "";
  }
}

size_t token::get_number() {
  if(type == tokentype::number)
    return *((size_t*)value.get());
  else
    return 0;
}

std::string token::get_string() {
  return get_word();
}

char token::get_symbol() {
  if(type == tokentype::symbol)
    return *((char*)value.get());
  else
    return '0';
}

char token::get_char() {
  if(type == tokentype::character)
    return *((char*)value.get());
  else
    return '0';
}

lexer::lexer() {
  tokens = std::vector<token>();
  code = "";
}

char lexer::currentchar() {
  return charat(index);
}

char lexer::charat(size_t pos) {
  return code[pos];
}

void lexer::next() {
  index++;
  col++;
  if(is_newline()) {
    line++;
    col = 0;
  }
}

void lexer::scan_all() {
  index = 0;
  line = col = 0;
  while(!is_eof()) {
    if(is_digit()) {
      scan_number();
    } else if (is_char()) {
      scan_word();
    } else if (is_quote()) {
      scan_string();
    } else if (is_space()) {
      //skip this
      next();
    } else {
      scan_symbol();
    }
  }
}

void lexer::scan_word() {
  std::ostringstream ss;
  while (is_char()) {
    ss << currentchar();
    next();
  }

  //create token and add to list
  token t{};
  t.type = tokentype::word;
  t.value = std::make_shared<std::string>(ss.str());
  t.c = col;
  t.l = line;

  tokens.push_back(t);
}

void lexer::scan_number() {
  std::ostringstream ss;

  while (is_digit()) {
    ss << currentchar();
    next();
  }

  float num = stoi(ss.str());

  token t{};
  t.type = tokentype::number;
  t.value = std::make_shared<size_t>(num);
  t.c = col;
  t.l = line;

  tokens.push_back(t);
}

void lexer::scan_char() {
  if(!is_squote())
    return;

  next(); //skip the first quote

  token t{};
  t.type = tokentype::character;
  t.value = std::make_shared<char>(currentchar());
  t.c = col;
  t.l = line;

  next(); //skip char
  next(); //skip end quote

  tokens.push_back(t);

}

void lexer::scan_string() {
  if(!is_quote())
    return;
  next();

  std::ostringstream ss;

  while (!is_quote()) {
    if(is_escape()) {
      next();
      switch (currentchar()) {
        case 'n':
          ss << '\n';
          break;
        case '"':
          ss << '"';
          break;
        case 't':
          ss << '\t';
          break;
        case '\'':
          ss << '\'';
          break;
        case '0':
          ss << '\0';
          break;
        case '\\':
          ss << '\\';
          break;
        case 'r':
          ss << '\r';
          break;
        case 'b':
          ss << '\b';
          break;
        default:
          ss << currentchar();
          break;
      }
    } else {
      ss << currentchar();
    }
    next();
  }

  next(); //skip the final " so we don't parse the next token as a string

  token t{};
  t.type = tokentype::string;
  t.value = std::make_shared<std::string>(ss.str());
  t.c = col;
  t.l = line;

  tokens.push_back(t);
}

void lexer::scan_symbol() {
  token t{};
  t.type = tokentype::symbol;
  t.value = std::make_shared<char>(currentchar());
  t.c = col;
  t.l = line;

  next();

  tokens.push_back(t);
}

bool lexer::is_digit() {
  return isdigit(currentchar());
}

bool lexer::is_char() {
  return isalpha(currentchar());
}

bool lexer::is_quote() {
  return currentchar() == '"';
}

bool lexer::is_newline() {
  return currentchar() == '\n';
}

bool lexer::is_escape() {
  return currentchar() == '\\';
}

bool lexer::is_eof() {
  return index >= code.length();
}

bool lexer::is_space() {
  return currentchar() == ' ';
}

bool lexer::is_squote() {
  return currentchar() == '\'';
}

bool lexer::is_minus() {
  return currentchar() == '-';
}

#endif
