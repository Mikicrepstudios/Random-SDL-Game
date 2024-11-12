#pragma once

#include "presets.h"
#include "settings.h"

namespace player {
	void Movement(SDL_Event event, game::Map &map, game::Player &player);
}
