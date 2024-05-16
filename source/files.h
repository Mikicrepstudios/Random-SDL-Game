#pragma once
#include "block.h"

namespace files {
	extern void SaveMap(SDL_Event event, Block worldMap[250][250], int mapWidth, int mapHeight);
	extern void LoadMap(SDL_Event event, Block worldMap[250][250], int mapWidth, int mapHeight);
}
