#include <fstream>

#include "block.h"
#include "settings.h"

namespace files {
	void SaveMap(game::Map map) {
		// Open file
		std::ofstream blocktypesFile("save/maptypes.msave");
		std::ofstream blockcolorsFile("save/mapcolors.msave");

		std::ofstream blocksolidsFile("save/mapsolids.msave");

		// Save blocktype
		if (blocktypesFile.is_open()) {
			for (int x = 0; x <= map.width; x++) {
				for (int y = 0; y <= map.height; y++) {
					if (map.map[x][y].type != 1) blocktypesFile << map.map[x][y].type << " "; // Write blocks
					else blocktypesFile << 0 << " "; // Do not write if player
				}

				blocktypesFile << "\n"; // Start a new line for the next row
			}

			blocktypesFile.close();
		}
		// Save colors
		if (blockcolorsFile.is_open()) {
			for (int x = 0; x <= map.width; x++) {
				for (int y = 0; y <= map.height; y++) {
					if (map.map[x][y].type != 1) blockcolorsFile << map.map[x][y].color << " "; // Write blocks
					else blockcolorsFile << 0 << " "; // Do not write if player
				}

				blockcolorsFile << "\n"; // Start a new line for the next row
			}

			blockcolorsFile.close();
		}

		// Save solidity
		if (blocksolidsFile.is_open()) {
			for (int x = 0; x <= map.width; x++) {
				for (int y = 0; y <= map.height; y++) {
					blocksolidsFile << map.map[x][y].isSolid << " "; // Write blocks
				}

				blocksolidsFile << "\n"; // Start a new line for the next row
			}

			blocksolidsFile.close();
		}
	}

	void LoadMap(game::Map &map) {
		// Open file
		std::ifstream blocktypesFile("save/maptypes.msave");
		std::ifstream blockcolorsFile("save/mapcolors.msave");

		std::ifstream blocksolidsFile("save/mapsolids.msave");

		// Load blocktypes
		if (blocktypesFile.is_open()) {
			for (int x = 0; x <= map.width; x++) {
				for (int y = 0; y <= map.height; y++) {
					blocktypesFile >> map.map[x][y].type;
				}
			}
			blocktypesFile.close();
		}
		// Load colors
		if (blockcolorsFile.is_open()) {
			for (int x = 0; x <= map.width; x++) {
				for (int y = 0; y <= map.height; y++) {
					blockcolorsFile >> map.map[x][y].color;
				}
			}
			blockcolorsFile.close();
		}

		// Load solids
		if (blocksolidsFile.is_open()) {
			for (int x = 0; x <= map.width; x++) {
				for (int y = 0; y <= map.height; y++) {
					blocksolidsFile >> map.map[x][y].isSolid;
				}
			}
			blocksolidsFile.close();
		}
	}
}
