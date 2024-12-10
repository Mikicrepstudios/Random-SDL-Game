#pragma once

#include "mf/core.h"

#include "settings.h"

/**
 * @brief Namespace that contains all game cheats
 */
namespace cheats {
	int CamTp(core::MF_Window &window, game::Game game, game::Settings &settings, game::Camera &cam);
	int PlayerTp(core::MF_Window &window, game::Game game, game::Settings &settings, game::Map &map, game::Camera &camera, game::Player &cam);
}
