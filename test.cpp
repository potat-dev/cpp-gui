#include <iostream>
#include <stdexcept>

#include "src/gui.hpp"

int main() {
  GUI gui;
  try {
    while (gui.isRunning()) gui.Update();
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}