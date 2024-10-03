#pragma once

#include "settings.h"

namespace cheats {
	extern int CamTp(game::SDL_Settings sdlSettings, game::Settings &settings, game::Camera &cam);
	extern int PlayerTp(game::SDL_Settings sdlSettings, game::Settings &settings, game::Map &map, game::Camera &camera, game::Player &cam);
}
