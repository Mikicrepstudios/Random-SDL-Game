#include "settings.h"

namespace Database {
void InitItems(game::Game &game) {
  auto &items = game.database.items;

  items[1].name = "Wood";
  items[1].itemID = 1;
  items[1].type = 1;
  items[1].blockID = 11;
}
} // namespace Database

/*
ITEM TYPES:
0 - item (Does nothing on its own)
1 - block (Can be placed)
*/