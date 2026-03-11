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

  const int iSize = 50; // Item size
  const int bSize = 5;  // Border size

  SDL_Rect hotbarBGrect = {
      0,
      window.height / 2 -
          ((10 * (iSize + bSize)) * settings.uiScale * settings.hSFactor / 2) -
          bSize,
      (iSize + 2 * bSize) * settings.uiScale * settings.wSFactor,
      ((10 * (iSize + bSize)) * settings.uiScale * settings.hSFactor) + bSize};
  draw::DrawRect(window.renderer, hotbarBGrect, colors::darkgray); // Draw bg

  SDL_Rect selHotbarRect = {
      0,
      window.height / 2 -
          ((10 * (iSize + bSize)) * settings.uiScale * settings.hSFactor / 2) +
          game.player.curInventorySlot * (iSize + bSize) - bSize,
      iSize + 2 * bSize, iSize + 2 * bSize};
  draw::DrawRect(window.renderer, selHotbarRect,
                 colors::lightgray); // Highlight selected slot

  SDL_Rect curRect = {};
  SDL_Rect curSmallRect = {};
  for (int i = 0; i < 10; i++) {
    curRect.x = bSize;
    curRect.y =
        window.height / 2 -
        ((10 * (iSize + bSize)) * settings.uiScale * settings.hSFactor / 2) +
        i * (iSize + bSize),
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
      graphics::DrawBlock(window, game.database.items[inventory[i].id], curRect,
                          blockTextures);

      // Draw amount
      draw::DrawText(window.renderer, window.font, curSmallRect,
                     std::to_string(inventory[i].amount).c_str(),
                     colors::white);
    }
  }
}
} // namespace inventory