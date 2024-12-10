#pragma once

#include "block.h"
#include "game.h"
#include "settings.h"

/**
 * @brief Namespace that contains all functions that are related to managing files on disk
 */
namespace files {
	void SaveMap(game::Map map);
	void SaveSettings(game::Settings settings, game::Player player, game::Camera cam);
	void LoadMap(game::Map &map);
	void LoadSettings(game::Settings &settings, game::Player &player, game::Camera &cam, game::Preset preset[10]);
}	
