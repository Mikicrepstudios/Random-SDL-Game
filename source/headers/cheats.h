#pragma once

#include "settings.h"

namespace cheats {
	extern void CamTp(game::SDL_Settings sdlSettings, game::Settings &settings, game::Camera &cam);
	extern void PlayerTp(game::SDL_Settings sdlSettings, game::Settings &settings, game::Map &map, game::Camera &camera, game::Player &cam);
}
