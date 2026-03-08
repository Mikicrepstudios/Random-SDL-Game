#include "settings.h"

namespace map {
void PlaceBlock(game::Game &game, int x, int y) {
  game.map.map[x][y].type = 2;
  game.map.map[x][y].color = game.settings.blockColor;
  game.map.map[x][y].texture = game.settings.blockTextureId;
  game.map.map[x][y].isSolid = game.settings.placeSolidBlocks;
}
} // namespace map