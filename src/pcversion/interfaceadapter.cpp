#include "pcversion/interfaceadapter.h"
#include <chrono>
#include <thread>



std::pair<Interface*,InterfaceData*> getInterface ()
{
  Interface *interface = new Interface;
  InterfaceData *if_data = new InterfaceData;

  interface->readButtons = [if_data] () {return if_data->buttons;};
  interface->print = [](std::string data) {std::cout << data;};
  interface->userHexInt = []() {int n; std::cin >> std::hex >> n; return n;};
  interface->sleepMillis = [] (int ms) {std::this_thread::sleep_for(std::chrono::milliseconds(ms));};
  interface->realTimeMicros = [if_data] () {return timeMicros(if_data);};
  interface->updateScreen = [if_data] (ScreenFrame *sf) {updateScreen(if_data, sf);};

  return std::make_pair(interface, if_data);
}


void updateButtons (InterfaceData* if_data, Byte buttons_pressed)
{
  if_data->buttons = buttons_pressed;
}


void getNewScreen (InterfaceData* if_data, ScreenPixels &sp)
{
  if_data->screen_mutex.lock();
  for (Byte row = 0; row < SCREEN_PX_H; row++) {
    for (Byte col = 0; col < SCREEN_PX_W; col++) {
      sp[row][col] = if_data->screen[row][col];
    }
  }
  if_data->screen_mutex.unlock();
}