#pragma once

#include "presets.h"
#include "settings.h"

/**
 * @brief Namespace that contains all player related stuff
 */
namespace player {
	void Movement(SDL_Event event, game::Map &map, game::Player &player);
}
