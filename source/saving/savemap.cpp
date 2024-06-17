#include <fstream>

#include "block.h"

namespace files {
	void SaveMap(Block worldMap[250][250], int mapWidth, int mapHeight) {
		// Open file
		std::ofstream blocktypesFile("save/map1.msave");
		std::ofstream blockcolorsFile("save/map2.msave");

		// Save blocktype
		if (blocktypesFile.is_open()) {
			for (int x = 0; x <= mapWidth; x++) {
				for (int y = 0; y <= mapHeight; y++) {
					if (worldMap[x][y].type != 1) {
						blocktypesFile << worldMap[x][y].type << " "; // Write blocks
					}
					else {
						blocktypesFile << 0 << " "; // Do not write if player
					}
				}
				blocktypesFile << "\n"; // Start a new line for the next row
			}
			blocktypesFile.close();
		}
		// Save colors
		if (blockcolorsFile.is_open()) {
			for (int x = 0; x <= mapWidth; x++) {
				for (int y = 0; y <= mapHeight; y++) {
					if (worldMap[x][y].type != 1) {
						blockcolorsFile << worldMap[x][y].color << " "; // Write blocks
					}
					else {
						blockcolorsFile << 0 << " "; // Do not write if player
					}
				}
				blockcolorsFile << "\n"; // Start a new line for the next row
			}
			blockcolorsFile.close();
		}
	}

	void LoadMap(Block worldMap[250][250], int mapWidth, int mapHeight) {
		// Open file
		std::ifstream blocktypesFile("save/map1.msave");
		std::ifstream blockcolorsFile("save/map2.msave");

		// Load blocktypes
		if (blocktypesFile.is_open()) {
			for (int x = 0; x <= mapWidth; x++) {
				for (int y = 0; y <= mapHeight; y++) {
					blocktypesFile >> worldMap[x][y].type;
				}
			}
			blocktypesFile.close();
		}
		// Load colors
		if (blockcolorsFile.is_open()) {
			for (int x = 0; x <= mapWidth; x++) {
				for (int y = 0; y <= mapHeight; y++) {
					blockcolorsFile >> worldMap[x][y].color;
				}
			}
			blockcolorsFile.close();
		}
	}
}
