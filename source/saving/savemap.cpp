#include <fstream>

#include "block.h"
#include "settings.h"

namespace files {
	void SaveMap(game::Map map) {
		/**
		 * @brief This function saves map to file(s)
		 * @param map Game map
		 */

		// Open file
		std::ofstream blocktypesFile("save/maptypes.msave");
		std::ofstream blockcolorsFile("save/mapcolors.msave");
		std::ofstream blocktexturesFile("save/maptextures.msave");

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
		// Save textures
		if (blocktexturesFile.is_open()) {
			for (int x = 0; x <= map.width; x++) {
				for (int y = 0; y <= map.height; y++) {
					if (map.map[x][y].type != 1) blocktexturesFile << map.map[x][y].texture << " "; // Write blocks
					else blocktexturesFile << 0 << " "; // Do not write if player
				}

				blocktexturesFile << "\n"; // Start a new line for the next row
			}

			blocktexturesFile.close();
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
		/**
		 * @brief This function loads saved map
		 * @param map Game map
		 */
		
		// Open file
		std::ifstream blocktypesFile("save/maptypes.msave");
		std::ifstream blockcolorsFile("save/mapcolors.msave");
		std::ifstream blocktexturesFile("save/maptextures.msave");

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
		// Load textures
		if (blocktexturesFile.is_open()) {
			for (int x = 0; x <= map.width; x++) {
				for (int y = 0; y <= map.height; y++) {
					blocktexturesFile >> map.map[x][y].texture;
				}
			}
			blocktexturesFile.close();
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
