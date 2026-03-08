#include "settings.h"

namespace map {
void DestroyBlock(game::Game &game, int x, int y) {
  game.map.map[x][y].type = 0;
  game.map.map[x][y].color = 0;
  game.map.map[x][y].texture = 0;
  game.map.map[x][y].isSolid = false;
}
} // namespace map