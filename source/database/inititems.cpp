#include "settings.h"

namespace Database {
void InitItems(game::Game &game) {
  auto &items = game.database.items;

  items[11].id = 11;
  items[11].name = "Wood";
  items[11].color = 17;
  items[11].texture = 5;
}
} // namespace Database