#include "settings.h"

namespace map {
void PlaceBlock(game::Game &game, game::Game::Database::Blocks &block, int x,
                int y) {
  game.map.map[x][y].type = block.id;
  game.map.map[x][y].color = block.color;
  game.map.map[x][y].texture = block.texture;
  game.map.map[x][y].isSolid = block.isSolid;
}
} // namespace map