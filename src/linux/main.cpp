#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <chrono>
#include "emulator/cpu/cpu.h"
#include "emulator/utils/initialization.h"
#include "emulator/utils/debug.h"
#include "tetris.h"


void testGraphics ()
{
  // Dimensions of the window and the grid
  const int windowWidth = 400;
  const int windowHeight = 400;
  const int rows = 10;
  const int cols = 10;
  const int squareSize = windowWidth / cols;  // Adjust size of squares based on window size

  // Example data for colors (values in range [0, 255])
  std::vector<std::vector<int>> colorValues(rows, std::vector<int>(cols));
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      colorValues[i][j] = (i + j) * 20; // Example color gradient based on position
    }
  }

  // Create a window
  sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Matrix of Colored Squares");

  // Main loop to render the window
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();  // Clear the window

    // Draw each square with a color based on the values in the array
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
        int colorValue = colorValues[i][j];
        square.setFillColor(sf::Color(colorValue, 255 - colorValue, 128));  // RGB color based on value
        square.setPosition(j * squareSize, i * squareSize);
        window.draw(square);
      }
    }

    window.display();  // Display the drawn content
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
}


int main()
{
  testEmulator();
  return 0;
}