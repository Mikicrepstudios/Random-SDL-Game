#pragma once

#include "block.h"

namespace files {
	extern void SaveMap(Block worldMap[250][250], int mapWidth, int mapHeight);
	extern void SaveSettings(int playerX, int playerY, int camOffSetX, int camOffSetY, int playerColor, int blockColor, int bgColor);
	extern void LoadMap(Block worldMap[250][250], int mapWidth, int mapHeight);
	extern void LoadSettings(int &playerX, int &playerY, int &camOffSetX, int &camOffSetY, int &playerColor, int &blockColor, int &bgColor);
}	
