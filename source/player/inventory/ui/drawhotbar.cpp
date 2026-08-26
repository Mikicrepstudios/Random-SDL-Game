#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"

#include "graphics.h"
#include "inventory.h"
#include "textures.h"

namespace inventory {
void DrawHotbar(core::MF_Window &window, game::Game &game,
                textures::BlockTextures blockTextures[32]) {
  auto &settings = game.settings;
  auto &inventory = game.player.inventory;

  const float scale = (settings.hSFactor + settings.wSFactor) / 2.0f;
  const int iSize = static_cast<int>(50.0f * settings.uiScale * scale);
  const int bSize = static_cast<int>(5.0f * settings.uiScale * scale);
  const int slotStride = iSize + bSize;
  const int hotbarWidth = iSize + 2 * bSize;
  const int hotbarHeight = 10 * slotStride;
  const int hotbarTop = window.height / 2 - hotbarHeight / 2;

  SDL_Rect hotbarBGrect = {0, hotbarTop, hotbarWidth, hotbarHeight};
  draw::DrawRect(window.renderer, hotbarBGrect, colors::darkgray); // Draw bg

  SDL_Rect selHotbarRect = {
      0, hotbarTop + game.player.curInventorySlot * slotStride - bSize,
      hotbarWidth, hotbarWidth};
  draw::DrawRect(window.renderer, selHotbarRect,
                 colors::lightgray); // Highlight selected slot

  SDL_Rect curRect = {};
  SDL_Rect curSmallRect = {};
  for (int i = 0; i < 10; i++) {
    curRect.x = bSize;
    curRect.y = hotbarTop + i * slotStride;
    curRect.w = iSize;
    curRect.h = iSize;

    // Draw black bg
    draw::DrawRect(window.renderer, curRect,
                   colors::black); // TODO: Make this only on ID 0 when
                                   // implementing drawing item preview
    if (inventory[i].id != 0) {
      curSmallRect.x = curRect.x + (iSize / 3 * 2);
      curSmallRect.y = curRect.y + (iSize / 3 * 2);
      curSmallRect.w = iSize / 3;
      curSmallRect.h = iSize / 3;

      // Draw item
      if (game.database.items[inventory->id].type == 1)
        graphics::DrawBlock(
            window,
            game.database.blocks[game.database.items[inventory->id].blockID],
            curRect, blockTextures);
      else
        draw::DrawRect(window.renderer, curRect,
                       colors::aqua); // Replace this with drawing actual item

      // Draw amount
      draw::DrawText(window.renderer, window.font, curSmallRect,
                     std::to_string(inventory[i].amount).c_str(),
                     colors::white);
    }
  }
}
} // namespace inventory