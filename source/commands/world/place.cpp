#include <iostream>
#include <sstream>
#include <string>

#include "map.h"
#include "settings.h"

namespace commands {
int Place(std::string args, int argsCount, game::Game &game) {
  /**
   * @brief Function for Place command
   * @param args Command arguments
   * @param argsCount Amount of command arguments
   * @param settings Game settings
   * @param map Game map
   */
  // Empty command
  if (argsCount == 0 || argsCount == 1) {
    return 2;
  }

  std::string *argsArray = new std::string[argsCount];
  std::stringstream ss(args);

  std::string curWord;
  int index = 0;
  while (ss >> curWord) {
    if (index < argsCount) {
      argsArray[index++] = curWord;
    }
  }

  int x = std::stoi(argsArray[0]);
  int y = std::stoi(argsArray[1]);

  // place x y
  if (argsCount == 2) {
    map::PlaceBlock(game, x, y);
    std::cout << "Successfuly placed block" << std::endl;

    return 0;
  }

  if (argsCount > 2) {
    return 3;
  }

  return -69;
}
} // namespace commands