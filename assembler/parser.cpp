#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"

#ifndef PARSER_C
#define PARSER_C

parser::parser() {
  nodes = std::vector<ast::ast_node>();
}

token parser::currenttoken() {
  return tokens[index];
}

void parser::next() {
  index++;
}

bool parser::is_word() {
  return currenttoken().type == tokentype::word;
}

bool parser::is_number() {
  return currenttoken().type == tokentype::number;
}

bool parser::is_string() {
  return currenttoken().type == tokentype::string;
}

bool parser::is_symbol() {
  return currenttoken().type == tokentype::symbol;
}

bool parser::is_char() {
  return currenttoken().type == tokentype::character;
}

void parser::parse_all() {

}

ptr<ast::register_a> parser::parse_register() {
  if(is_word()) {
    std::string wrd = currenttoken().get_word();

  }
  throw parse_exception{index,"Token is not word, cannot parse register"};
}

ptr<ast::number_a> parser::parse_number() {
  if(is_number()) {
    ptr<ast::number_a> n = std::make_shared<ast::number_a>();
    n->num = currenttoken().get_number();
    return n;
  }
  throw parse_exception{index,"Token is not number."};
}

ptr<ast::word_a> parser::parse_word() {
  if(is_word()) {
    ptr<ast::word_a> w = std::make_shared<ast::word_a>();
    w->word = currenttoken().get_word();
    return w;
  }
  throw parse_exception{index,"Token is not a word, cannot parse word"};
}

ptr<ast::char_a> parser::parse_char() {
  if(is_char()) {
    ptr<ast::char_a> c = std::make_shared<ast::char_a>();
    c->ch = currenttoken().get_char();
    return c;
  }
  throw parse_exception{index,"Token is not character"};
}

ptr<ast::int_a> parser::parse_int() {
  bool neg = false;
  ptr<ast::number_a> n;
  if(is_symbol()) {
    if(currenttoken().get_symbol() == '-') {
      next();
      neg = true;
    }
    throw parse_exception{index,"unknown symbol for int"};
  } else if (is_number()) {
    neg = false;
  }
  n = parse_number();
  n->is_neg = neg;
  return n;
}

ptr<ast::label_a> parser::parse_label() {
  ptr<ast::word_a> w;
  if(is_word()) {
    w = parse_word();
    next();
    if(is_symbol() && currenttoken().get_symbol() == ':')
    {
      next();
    } else {
      throw parse_exception{index,"Token is not ':', cannot parse label"};
    }
    return w;
  } else {
    throw parse_exception{index,"Token is not word, cannot parse label"};
  }
}

#endif
