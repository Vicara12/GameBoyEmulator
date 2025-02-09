/*
* Functions for executing the PC version of the emulator
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <thread>
#include "pcversion/interfaceadapter.h"


void interfaceLoop (InterfaceData *if_data, std::thread &emulation_thread);

// Returns wether the user wants to terminate the program
bool handleInputs (sf::RenderWindow &window, InterfaceData *if_data);

void drawScreen (int px_size, sf::RenderWindow &window, InterfaceData *if_data);