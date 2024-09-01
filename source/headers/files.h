#pragma once

#include "block.h"
#include "settings.h"

namespace files {
	extern void SaveMap(game::Map map);
	extern void SaveSettings(game::Settings settings, game::Player player, game::Camera cam);
	extern void LoadMap(game::Map &map);
	extern void LoadSettings(game::Settings &settings, game::Player &player, game::Camera &cam);
}	
