#include <iostream>

#include "lexer.hpp"

#include "lexer.cpp"

using namespace std;

int main() {
  std::string code;
  getline(cin, code);

  lexer l{};
  l.code = code;

  l.scan_all();

  for(token t : l.tokens) {
    cout << t.type << " at " << t.l << ":" << t.c << " = ";
    if(t.type == tokentype::word)
      cout << t.get_word() << endl;
    else if (t.type == tokentype::number)
      cout << t.get_number() << endl;
    else if (t.type == tokentype::string)
      cout << t.get_string() << endl;
    else if (t.type == tokentype::character)
      cout << t.get_char() << endl;
    else if (t.type == tokentype::symbol)
      cout << t.get_symbol() << endl;
  }

  return 0;
}
