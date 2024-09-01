#pragma once

#include "presets.h"
#include "settings.h"

namespace player {
	extern void PlayerMovement(SDL_Event event, game::Map &map, game::Player &player);
}
