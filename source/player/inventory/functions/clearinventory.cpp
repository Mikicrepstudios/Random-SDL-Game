#include "settings.h"

namespace inventory {
void ClearInventory(game::Game &game) {
  for (int i = 0; i < game.player.inventorySize; i++) {
    game.player.inventory[i].id = 0;
    game.player.inventory[i].amount = 0;
  }
}
} // namespace inventory