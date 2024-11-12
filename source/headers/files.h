#pragma once

#include "block.h"
#include "presets.h"
#include "settings.h"

namespace files {
	void SaveMap(game::Map map);
	void SaveSettings(game::Settings settings, game::Player player, game::Camera cam);
	void LoadMap(game::Map &map);
	void LoadSettings(game::Settings &settings, game::Player &player, game::Camera &cam, game::Preset preset[10]);
}	
