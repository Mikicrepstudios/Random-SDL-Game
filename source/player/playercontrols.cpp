#include "SDL.h"

#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"

#include "block.h"
#include "game.h"
#include "map.h"
#include "settings.h"

#include <iostream>

namespace game {
void HandleLeftClick(game::Game &game, int mapX, int mapY) {
  auto &settings = game.settings;
  auto &map = game.map;

  if (!settings.cheats)
    map::PlaceBlock(game, mapX, mapY);
}

void HandleRightClick(game::Game &game, int mapX, int mapY) {
  auto &settings = game.settings;

  if (!settings.cheats)
    map::DestroyBlock(game, mapX, mapY);
  else
    settings.cheats = false;
}

void MouseEvent(core::MF_Window &window, game::Game &game) {
  /**
   * @brief This function controls block placing and destroying using mouse
   */

  auto &settings = game.settings;
  auto &cam = game.cam;
  auto &map = game.map;

  // Calculate map-relative coordinates by adding positive camera offset
  int mapX = game.curHoverX + cam.offSetX;
  int mapY = game.curHoverY + cam.offSetY;

  if (window.mouse.isDown && !settings.inventory &&
      map.map[mapX][mapY].type != 1) // Check for most basic stuff
    if (mapX >= 0 && mapX < map.width && mapY >= 0 &&
        mapY < map.height) { // Dont allow to go out of bounds
      if (window.mouse.leftButtonPressed)
        HandleLeftClick(game, mapX, mapY);

      if (window.mouse.rightButtonPressed)
        HandleRightClick(game, mapX, mapY);
    }
}
} // namespace game

// TMP for cleanup
// Q Color picker and stuff (unimplemented) (needs also preset[10])
/*else if (window.mouse.isDown && settings.cheats && settings.cheatsId == 3) {
if (map.map[mapX][mapY].type == 2) {
preset[settings.curPreset].blockColor =
map.map[mapX][mapY].color; preset[settings.curPreset].textureId =
map.map[mapX][mapY].texture; settings.cheats = false;
}
}*/
// void MouseOverlay(core::MF_Window &window, game::Game &game) {

/**
 * @brief This function shows which block are you hovering
 */

/*auto& settings = game.settings;
auto& cam = game.cam;
auto& map = game.map;

int curHoverX = game.curHoverX;
int curHoverY = game.curHoverY;

int x = curHoverX + cam.offSetX; // subtract positive offset
int y = curHoverY + cam.offSetY; // subtract positive offset

// Check bounds including non-negativity and max limits
if (x >= 0 && x < map.width &&
        y >= 0 && y < map.height &&
        map.map[x][y].type != 1 &&
        !settings.inventory)
{
        SDL_Rect mouseRect = {curHoverX * cam.scale, curHoverY * cam.scale,
cam.scale, cam.scale};

        if (!cam.highlight) {
                if (settings.bgColor == 32 || map.map[x][y].color == 32)
                        draw::DrawRect(window.renderer, mouseRect,
colors::black); else draw::DrawRect(window.renderer, mouseRect,
colors::white); } else { if (settings.bgColor == 27)
                        draw::DrawRect(window.renderer, mouseRect,
colors::darkred); else draw::DrawRect(window.renderer, mouseRect,
colors::red);
        }
}
}
}*/