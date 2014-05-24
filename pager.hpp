#pragma once
#include "head.hpp"

enum page_mode
{
  user_mode,
  root_mode
};

struct page
{
  maddr address{0};
  bool present{false};
  bool writeable{false};
  page_mode mode{user_mode};
  bool nocache{true};
};

struct pageTable
{
  bool present{false};
  bool writeable{false};
  page_mode mode{user_mode};
  bool nocache{true};

  std::vector<page> pages{PAGE_TABLE_SIZE};

  std::vector<page>::iterator get_page(int index);
};

class pager
{
  std::vector<pageTable> cache{PAGE_DIRECTORY_SIZE};
  int PT{0};
public:

  std::vector<pageTable>::iterator get_page_table(int index);

  bool can_read(maddr address);
  bool is_present(int index);
};
