#include "settings.h"

namespace inventory {
void RemoveItem(game::Game &game, int id, int amount) {
  auto &inventory = game.player.inventory;
  const int &inventorySize = game.player.inventorySize;

  // Try to find item
  for (int i = 0; i < inventorySize; i++) {
    if (inventory[i].id == id) {
      inventory[i].amount -= amount;

      // Reset item to air if amount gets to/under 0
      if (inventory[i].amount <= 0) {
        inventory[i].id = 0;
        inventory[i].amount = 0;
      }
    }
  }

  // TODO: Handle error if needed?
}
} // namespace inventory