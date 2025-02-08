#include <SFML/Graphics.hpp>
#include <thread>
#include "pcversion/interfaceadapter.h"
#include "emulator/emulator.h"
#include "emulator/multimedia/graphicstate.h"
#include "tetris.h"


void drawScreen (int px_size, sf::RenderWindow &window, InterfaceData *if_data)
{
  ScreenPixels px_intensity(SCREEN_PX_H, std::vector<float>(SCREEN_PX_W));
  getNewScreen(if_data, px_intensity);
  std::array<float,3> base_color = {255, 255, 255};

  window.clear();
  for (int i = 0; i < SCREEN_PX_H; i++) {
    for (int j = 0; j < SCREEN_PX_W; j++) {
      sf::RectangleShape square(sf::Vector2f(px_size, px_size));
      square.setFillColor(sf::Color(px_intensity[i][j]*base_color[0],
                                    px_intensity[i][j]*base_color[1],
                                    px_intensity[i][j]*base_color[2]));
      square.setPosition(j * px_size, i * px_size);
      window.draw(square);
    }
  }
  window.display();
}


void interfaceLoop (InterfaceData *if_data, std::thread &emulation_thread)
{
  int px_size = 4;
  // Dimensions of the window and the grid
  const int windowWidth = px_size*SCREEN_PX_W;
  const int windowHeight = px_size*SCREEN_PX_H;

  // Create a window
  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Game Boy");

  // Main loop to render the window
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        endEmulation(if_data);
        if (emulation_thread.joinable()) {
          emulation_thread.join();
        }
        window.close();
      }
    }

    drawScreen(px_size, window, if_data);
  }
}


int main ()
{
  std::pair<Interface*,InterfaceData*> if_pair = getInterface();
  Interface *interface = if_pair.first;
  InterfaceData *if_data = if_pair.second;
  std::thread emulation_thread(emulator, interface, &tetris_rom);
  emulation_thread.detach();
  interfaceLoop(if_data, emulation_thread);
}