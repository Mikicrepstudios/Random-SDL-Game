#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"

#include "settings.h"

namespace inventory {
void DrawInventory(core::MF_Window &window, game::Game::Settings &settings) {
  if (settings.inventory) {
    SDL_Rect bgRect = {
        65 * settings.uiScale * settings.wSFactor, // x
        window.height / 2 - 275 * settings.uiScale * settings.hSFactor -
            5,                                         // y top
        500 * settings.uiScale * settings.wSFactor,    // width
        550 * settings.uiScale * settings.hSFactor + 5 // height
    };
    draw::DrawRect(window.renderer, bgRect, colors::darkgray);
  }
}
} // namespace inventory