#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/glut.h>

#ifndef MAIN_H
#define MAIN_H

constexpr int DATA_BRIDGE_SIZE = 128;
constexpr int MEMORY_SIZE = 4*4*1024;
constexpr int FRAME_SIZE = 4*1024;

typedef unsigned char byte;
typedef unsigned int dword;

constexpr dword SCREEN_WIDTH = 800;
constexpr dword SCREEN_HEIGHT = 600;

//predefine for device pointer
class vm;

//Include headers
#include "utils.hpp"

#include "instruction.hpp"

#include "interupt.hpp"

#include "device.hpp"

#include "mapped_device.hpp"

#include "ram.hpp"

#include "cpu.hpp"

#include "databridge.hpp"

#include "CMOS.hpp"

#include "pager.hpp"

#include "psu.hpp"

#include "VGA.hpp"

#include "vm.hpp"

//Include functions
#include "vm.cpp"

#include "ram.cpp"

#include "device.cpp"

#include "cpu.cpp"

#include "databridge.cpp"

#include "psu.cpp"

#include "VGA.cpp"

#endif
