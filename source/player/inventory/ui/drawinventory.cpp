#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"

#include "settings.h"

namespace inventory {
void DrawInventory(core::MF_Window &window, game::Game::Settings &settings) {
  if (settings.inventory) {
    const float scale = (settings.hSFactor + settings.wSFactor) / 2.0f;
    const int borderSize = static_cast<int>(5.0f * settings.uiScale * scale);
    const int slotSize = static_cast<int>(50.0f * settings.uiScale * scale);
    const int slotStride = slotSize + borderSize;
    const int inventoryHeight = 10 * slotStride + borderSize;
    const int inventoryTop =
        window.height / 2 - inventoryHeight / 2 - borderSize;
    const int inventoryX =
        static_cast<int>(65.0f * settings.uiScale * settings.wSFactor);
    const int inventoryWidth =
        static_cast<int>(500.0f * settings.uiScale * settings.wSFactor);

    SDL_Rect bgRect = {inventoryX, inventoryTop, inventoryWidth,
                       inventoryHeight};
    draw::DrawRect(window.renderer, bgRect, colors::darkgray);
  }
}
} // namespace inventory