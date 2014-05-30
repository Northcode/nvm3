#pragma once
#include "head.hpp"
#include "ram.hpp"

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
public:
  int PT{0};

  std::vector<pageTable>::iterator get_page_table(int index);

  bool can_read(maddr address);
  bool can_write(maddr address);
  bool is_present(int index);
};

std::vector<pageTable>::iterator pager::get_page_table(int index) {
  return cache.begin() + index;
}

bool pager::can_read(maddr address) {
  int pi = address / PAGE_SIZE;
  auto p = get_page_table(pi);
  return (p->present && p->mode == user_mode);
}

bool pager::can_write(maddr address) {
  int pi = address / PAGE_SIZE;
  auto p = get_page_table(pi);
  return (p->present && p->writeable && p->mode == user_mode);
}

bool pager::is_present(int index) {
  auto p = get_page_table(index);
  return p->present;
}
