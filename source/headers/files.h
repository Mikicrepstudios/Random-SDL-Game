#pragma once

#include "block.h"
#include "settings.h"

namespace files {
	extern void SaveMap(Block worldMap[250][250], int mapWidth, int mapHeight);
	extern void SaveSettings(game::Settings settings, game::Player player, game::Camera camera, int blockColor);
	extern void LoadMap(Block worldMap[250][250], int mapWidth, int mapHeight);
	extern void LoadSettings(game::Settings &settings, game::Player &player, game::Camera &camera, int &blockColor);
}	
