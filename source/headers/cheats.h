#pragma once

#include "mf/core.h"

#include "settings.h"

namespace cheats {
	int CamTp(core::MF_Window &window, game::Game game, game::Settings &settings, game::Camera &cam);
	int PlayerTp(core::MF_Window &window, game::Game game, game::Settings &settings, game::Map &map, game::Camera &camera, game::Player &cam);
}
