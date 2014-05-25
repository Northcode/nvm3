#pragma once
#include "head.hpp"

constexpr int DIV_BY_ZERO{0};
constexpr int DEBUG_BREAKPOINT{1};
constexpr int INVALID_OPCODE{2};
constexpr int GP_FAULT{3};
constexpr int PAGE_FAULT{4};
constexpr int STACK_OVERFLOW{5};
constexpr int DOUBLE_FAULT{6};
constexpr int TRIPLE_FAULT{7};

constexpr int NEW_HARDWARE{8};
