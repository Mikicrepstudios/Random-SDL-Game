#include "settings.h"

namespace map {
void PlaceBlock(game::Game &game, game::Game::Database::Items &item, int x,
                int y) {
  game.map.map[x][y].type = item.id;
  game.map.map[x][y].color = item.color;
  game.map.map[x][y].texture = item.texture;
  game.map.map[x][y].isSolid = item.isSolid;
}
} // namespace map