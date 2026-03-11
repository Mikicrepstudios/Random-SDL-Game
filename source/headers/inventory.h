#ifndef INVENTORY_H
#define INVENTORY_H

#include "SDL.h"

#include "mf/core.h"

#include "game.h"
#include "settings.h"
#include "textures.h"

/**
 * @brief Namespace that contains all inventory related stuff
 */
namespace inventory {
// Basic actions
void ClearInventory(game::Game &game);
void AddItem(game::Game &game, int id, int amount);
void RemoveItem(game::Game &game, int id, int amount);

// Rendering
void DrawHotbar(core::MF_Window &window, game::Game &game,
                textures::BlockTextures blockTextures[32]);

// Extra
void HotbarSlotChooser(core::MF_Window &window, game::Game &game);
} // namespace inventory

#endif