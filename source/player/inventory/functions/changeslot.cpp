#include "SDL.h"

#include "mf/core.h"

#include "settings.h"

namespace inventory {
void HotbarSlotChooser(core::MF_Window &window, game::Game &game) {
  /**
   * @brief This function controls hotbar slot changing
   */

  SDL_Event &event = window.event;
  int &slot = game.player.curInventorySlot;

  if (event.key.keysym.sym == SDLK_1) {
    slot = 0;
  } else if (event.key.keysym.sym == SDLK_2) {
    slot = 1;
  } else if (event.key.keysym.sym == SDLK_3) {
    slot = 2;
  } else if (event.key.keysym.sym == SDLK_4) {
    slot = 3;
  } else if (event.key.keysym.sym == SDLK_5) {
    slot = 4;
  } else if (event.key.keysym.sym == SDLK_6) {
    slot = 5;
  } else if (event.key.keysym.sym == SDLK_7) {
    slot = 6;
  } else if (event.key.keysym.sym == SDLK_8) {
    slot = 7;
  } else if (event.key.keysym.sym == SDLK_9) {
    slot = 8;
  } else if (event.key.keysym.sym == SDLK_0) {
    slot = 9;
  }
}
} // namespace inventory
