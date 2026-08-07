#include "SDL.h"

#include "mf/core.h"

#include "settings.h"

namespace game {
void CameraControls(core::MF_Window &window, game::Game &game) {
  /**
   * @brief This function controls camera movement and other related stuff
   * @param window Game window
   * @param settings Game settings
   * @param cam Game camera
   */

  auto &cam = game.cam;
  auto &map = game.map;

  if (!game.settings.inventory) {
    if (window.event.key.keysym.sym == SDLK_UP)
      cam.offSetY -= 1;
    else if (window.event.key.keysym.sym == SDLK_LEFT)
      cam.offSetX -= 1;
    else if (window.event.key.keysym.sym == SDLK_DOWN)
      cam.offSetY += 1;
    else if (window.event.key.keysym.sym == SDLK_RIGHT)
      cam.offSetX += 1;
  }
  // Cam zoom out
  if ((window.event.key.keysym.sym == SDLK_o ||
       window.event.key.keysym.sym == SDLK_KP_MINUS) &&
      cam.scale != 10) {
    cam.scale -= 1;
  }
  // Cam zoom in
  else if (window.event.key.keysym.sym == SDLK_i ||
           window.event.key.keysym.sym == SDLK_KP_PLUS) {
    cam.scale += 1;
  }
  // Reset zoom
  else if (window.event.key.keysym.sym == SDLK_p) {
    cam.scale = 50;
    cam.offSetX = 0;
    cam.offSetY = 0;
  }
}
} // namespace game
