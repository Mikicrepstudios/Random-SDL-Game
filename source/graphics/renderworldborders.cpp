#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"

#include "settings.h"

namespace game {
void RenderWorldBorders(core::MF_Window &window, game::Game &game) {
  // This function draws red borders around world borders
  int borderSize = game.cam.scale / 5;

  // Calc rect which is size and position of world that is being drawn
  SDL_Rect worldRect = {};
  worldRect.x = -game.cam.offSetX * game.cam.scale;
  worldRect.y = -game.cam.offSetY * game.cam.scale;
  worldRect.w = game.map.width * game.cam.scale;
  worldRect.h = game.map.height * game.cam.scale;

  // Calc actual borders based on world rect
  SDL_Rect topRect = {};
  topRect.x = worldRect.x - borderSize;
  topRect.y = worldRect.y - borderSize;
  topRect.w = worldRect.w + borderSize * 2;
  topRect.h = borderSize;
  SDL_Rect rightRect = {};
  rightRect.x = worldRect.w - game.cam.offSetX * game.cam.scale;
  rightRect.y = worldRect.y;
  rightRect.w = borderSize;
  rightRect.h = worldRect.h + borderSize * 2;
  SDL_Rect bottomRect = {};
  bottomRect.x = worldRect.x - borderSize;
  bottomRect.y = worldRect.h - game.cam.offSetY * game.cam.scale + borderSize;
  bottomRect.w = worldRect.w + borderSize * 2;
  bottomRect.h = borderSize;
  SDL_Rect leftRect = {};
  leftRect.x = worldRect.x - borderSize;
  leftRect.y = worldRect.y;
  leftRect.w = borderSize;
  leftRect.h = worldRect.h + borderSize * 2;

  // Draw borders
  draw::DrawRect(window.renderer, topRect, colors::red);
  draw::DrawRect(window.renderer, rightRect, colors::red);
  draw::DrawRect(window.renderer, bottomRect, colors::red);
  draw::DrawRect(window.renderer, leftRect, colors::red);
}
} // namespace game