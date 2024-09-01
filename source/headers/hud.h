#pragma once

#include <string>
#include "settings.h"

namespace hud {
    extern void GameInfo(game::SDL_Settings sdlSettings, game::Settings settings, game::Camera cam, game::Player player);
}