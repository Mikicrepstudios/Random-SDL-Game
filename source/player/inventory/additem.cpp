#include "settings.h"

namespace inventory {
void AddItem(game::Game &game, int id, int amount) {
  auto &inventory = game.player.inventory;
  const int &inventorySize = game.player.inventorySize;

  // Try to stack
  for (int i = 0; i < inventorySize; i++) {
    if (inventory[i].id == id)
      inventory[i].amount += amount;
    return;
  }

  // Try to find empty slot
  for (int i = 0; i < inventorySize; i++) {
    if (inventory[i].id == 0) {
      inventory[i].id = id;
      inventory[i].amount = amount;
      return;
    }
  }

  // TODO: Handle error if needed?
}
} // namespace inventory