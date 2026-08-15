#include "settings.h"

namespace Database {
void InitBlocks(game::Game &game) {
  auto &blocks = game.database.blocks;

  blocks[11].name = "Wood";
  blocks[11].id = 11;
  blocks[11].color = 17;
  blocks[11].texture = 5;
  blocks[11].droppedItem = 1;
}
} // namespace Database