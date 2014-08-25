#!/bin/bash
#build nvm
echo "building nvm"
g++ -o nvm main.cpp --std=c++11 -g -lglut -lGL
echo "----------------"
echo "Building assembler"
g++ -o nvmasm assembler/main.cpp --std=c++11
echo "DONE!"
