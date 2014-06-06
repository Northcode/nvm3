g++ -o nvm3.o --std=c++11 -c -I ~/SFML-2.1/include main.cpp
g++ -o nvm3 nvm3.o -L ~/SFML-2.1/lib -lsfml-graphics -lsfml-window -lsfml-system
