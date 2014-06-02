#pragma once
#include "head.hpp"
#include "device.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class screen : public device
{
public:
  int height{600},width{800};
  sf::RenderWindow window{{width,height},"NVM3"};

  screen();
  ~screen();

  void update();

  void set_pixel(int x, int y, )
};

screen::screen() {

}

screen::~screen() {

}
