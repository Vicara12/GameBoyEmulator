#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <chrono>
#include "emulator/cpu/cpu.h"
#include "emulator/utils/initialization.h"
#include "emulator/utils/debug.h"
#include "tetris.h"
#include "pcversion/interfaceadapter.h"



void testGraphics (const std::vector<std::vector<float>> &px_intensity,
                   const std::array<float,3> &base_color)
{
  int px_size = 3;
  int cols = px_intensity[0].size();
  int rows = px_intensity.size();
  // Dimensions of the window and the grid
  const int windowWidth = px_size*cols;
  const int windowHeight = px_size*rows;

  // Create a window
  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Game Boy");

  // Main loop to render the window
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();

    // Draw each square with a color based on the values in the array
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
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
}

std::chrono::steady_clock::time_point timeNow ()
{
  return std::chrono::steady_clock::now();
}

std::chrono::microseconds timeDif (std::chrono::steady_clock::time_point ini_t)
{
  return std::chrono::duration_cast<std::chrono::microseconds>(timeNow() - ini_t);
}


void testEmulator ()
{
  std::cout << "Began testing" << std::endl;
  State *state = new State;
  Interface *interface = new Interface;
  auto ini_t = timeNow();
  auto micros = [ini_t] () -> ulong {return timeDif(ini_t).count();};
  interface->readButtons = [](){return 0;};
  interface->print = [](std::string data) {std::cout << data;};
  interface->logData = [](std::string data) {};
  loadBootRom(state);
  loadGame(state, &tetris_rom);
  std::cout << "Beginning execution..." << std::endl;
  auto t_ini = micros();
  execute(state, interface, 0x0100);
  auto t_fi = micros();
  float cycles = state->cycles;
  float t_theo = cycles*1e6/CLOCK_FREQ;
  std::cout << "Finished! Took: " << (t_fi - t_ini) << " us / " << ulong(t_theo) << " us max ("
            << 100*(t_fi - t_ini)/t_theo << "%)." << std::endl;
  std::cout << "State:" << std::endl;
  showRegisters(state, interface);
  showMemoryRange(state, 0xFF05, 0xFFFF, interface);

  ScreenPixels px_intensity;
  std::array<float,3> base_color = {1.0, 1.0, 1.0};
  screenFrameToScreenPixels(&(state->screen), px_intensity);
  testGraphics(px_intensity, base_color);
}


int main()
{
  testEmulator();
  return 0;
}