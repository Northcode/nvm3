#pragma once
#include "head.hpp"
#include "device.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

struct screen_color
{
  byte R,G,B;
};

class screen : public device
{
public:
  maddr height{600},width{800};
  sf::RenderWindow window{{width,height},"NVM3"};
  sf::Image vram;
  sf::Texture buffer;
  sf::Sprite sprite;

  screen();
  ~screen();

  void update();

  void set_pixel(maddr, maddr, screen_color);
};

screen::screen() {
  vram.create(height,width,sf::Color::Black);
  buffer = sf::Texture();
  buffer.loadFromImage(vram);
  sprite = sf::Sprite(buffer);
}

screen::~screen() {

}

void screen::update() {
  window.clear(sf::Color::Black);
  window.draw(sprite);
  window.display();
}

void screen::set_pixel(maddr x, maddr y, screen_color color) {
  vram.setPixel(x,y,sf::Color(color.R,color.G,color.B));
  buffer.loadFromImage(vram);
}
