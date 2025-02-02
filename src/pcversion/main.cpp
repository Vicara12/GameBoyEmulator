#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <chrono>
#include "emulator/cpu/cpu.h"
#include "emulator/utils/initialization.h"
#include "emulator/utils/debug.h"
#include "tetris.h"
#include "pcversion/interfaceadapter.h"
#include "emulator/multimedia/graphics.h"



void testGraphics (
                   std::array<std::array<float,256>,256> &px_intensity,
                  //  const ScreenPixels &px_intensity,
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


void testEmulator (Interface *interface)
{
  std::cout << "Began testing" << std::endl;
  State *state = new State;
  loadBootRom(state);
  loadGame(state, &tetris_rom);
  std::cout << "Beginning execution..." << std::endl;
  auto t_ini = interface->realTimeMicros();
  execute(state, interface, 0x0100);
  auto t_fi = interface->realTimeMicros();
  float cycles = state->cycles;
  float t_theo = cycles*1e6/CLOCK_FREQ;
  std::cout << "Finished! Took: " << (t_fi - t_ini) << " us / " << ulong(t_theo) << " us max ("
            << 100*(t_fi - t_ini)/t_theo << "%)." << std::endl;
  std::cout << "State:" << std::endl;
  showRegisters(state, interface);
  showMemoryRange(state, 0xFF05, 0xFF26, interface);
  showMemoryRange(state, 0xFF40, 0xFF4B, interface);
  showMemoryRange(state, 0xFFFF, 0xFFFF, interface);

  ScreenPixels px_intensity;
  std::array<float,3> base_color = {255, 255, 255};
  screenFrameToScreenPixels(&(state->screen), px_intensity);
  // testGraphics(px_intensity, base_color);
}


void debugBootRom (Interface *interface)
{
  State *state = runBootRomInDebug(interface, &tetris_rom);
  std::array<std::array<Byte,256>,256> bg;
  std::array<std::array<float,256>,256> bg_intensity;

  Short base_addr = 0x9800;
  // Short base_addr = 0x9C00;
  for (int row = 0; row < 32; row++) {
    for (int col = 0; col < 32; col++) {
      for (Byte tile_line = 0; tile_line < 8; tile_line++) {
        getTileLine(base_addr, tile_line, false, true, &(bg[0][0]) + (row*8 + tile_line)*256 + col*8, state);
      }
      base_addr++;
    }
  }
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      bg_intensity[i][j] = colorNumToFloat(bg[i][j]);
      std::cout << bg_intensity[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::array<float,3> base_color = {255, 255, 255};
  testGraphics(bg_intensity, base_color);
}


Interface* loadPCInterface ()
{
  Interface *interface = new Interface;
  auto ini_t = timeNow();
  interface->readButtons = [](){return 0;};
  interface->print = [](std::string data) {std::cout << data;};
  interface->logData = [](std::string data) {};
  interface->userHexInt = []() {int n; std::cin >> std::hex >> n; return n;};
  interface->realTimeMicros = [ini_t] () -> ulong {return timeDif(ini_t).count();};
  return interface;
}


int main()
{
  Interface *interface = loadPCInterface();
  debugBootRom(interface);
  // testEmulator(interface);
  return 0;
}