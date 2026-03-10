#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"

#include "inventory.h"

namespace inventory {
void DrawHotbar(core::MF_Window &window, game::Game &game) {
  auto &settings = game.settings;
  auto &inventory = game.player.inventory;

  int iSize = 50; // Item size
  int bSize = 5;  // Border size

  SDL_Rect hotbarBGrect = {
      0,
      window.height / 2 -
          ((10 * (iSize + bSize)) * settings.uiScale * settings.hSFactor / 2) -
          bSize,
      (iSize + 2 * bSize) * settings.uiScale * settings.wSFactor,
      ((10 * (iSize + bSize)) * settings.uiScale * settings.hSFactor) + bSize};
  draw::DrawRect(window.renderer, hotbarBGrect, colors::darkgray);

  for (int i = 0; i < 10; i++) {
    SDL_Rect curRect = {bSize,
                        window.height / 2 -
                            ((10 * (iSize + bSize)) * settings.uiScale *
                             settings.hSFactor / 2) +
                            i * (iSize + bSize),
                        50, 50};

    draw::DrawRect(window.renderer, curRect, colors::black);
  }
}
} // namespace inventory