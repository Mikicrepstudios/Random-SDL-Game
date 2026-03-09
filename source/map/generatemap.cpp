#include "mf/colors.h"
#include "mf/logic.h"

#include "map.h"
#include "settings.h"

namespace map {
void GenerateMap(game::Game &game) {
  int woodX = 0;
  int woodY = 0;

  map::ClearMap(game);

  for (int i = 0; i <= 50; i++) {
    woodX = logic::GenRanNum(0, game.map.width);
    woodY = logic::GenRanNum(0, game.map.height);

    game.map.map[woodX][woodY].type = 11;
    game.map.map[woodX][woodY].color = 17;
    game.map.map[woodX][woodY].texture = 5;
    game.map.map[woodX][woodY].isSolid = 1;
  }
}
} // namespace map