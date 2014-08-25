#include "main.hpp"

#ifndef PAGER_H
#define PAGER_H

union page_address {
  struct {
    dword offset:10;
    dword page:10;
    dword page_table:12;
  };
  dword raw;
};

union page_table {
  struct {
    bool present:1;
    bool mode:1;
    bool write:1;
    dword address:29;
  };
  dword raw;
};

union page_entry {
  struct {
    bool present:1;
    bool mode:1;
    bool write:1;
    dword address:29;
  };
  dword raw;
};

#endif
