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
		std::ofstream blockTypesFile("save/maptypes.msave");
		std::ofstream blockColorsFile("save/mapcolors.msave");
		std::ofstream blockTexturesFile("save/maptextures.msave");

		std::ofstream blockSolidsFile("save/mapsolids.msave");

		// Save blocktype
		if (blockTypesFile.is_open()) {
			for (int x = 0; x < map.width; x++) {
				for (int y = 0; y < map.height; y++) {
					if (map.map[x][y].type != 1) blockTypesFile << map.map[x][y].type << " "; // Write blocks
					else blockTypesFile << 0 << " "; // Do not write if player
				}

				blockTypesFile << "\n"; // Start a new line for the next row
			}

			blockTypesFile.close();
		}
		// Save colors
		if (blockColorsFile.is_open()) {
			for (int x = 0; x < map.width; x++) {
				for (int y = 0; y < map.height; y++) {
					if (map.map[x][y].type != 1) blockColorsFile << map.map[x][y].color << " "; // Write blocks
					else blockColorsFile << 0 << " "; // Do not write if player
				}

				blockColorsFile << "\n"; // Start a new line for the next row
			}

			blockColorsFile.close();
		}
		// Save textures
		if (blockTexturesFile.is_open()) {
			for (int x = 0; x < map.width; x++) {
				for (int y = 0; y < map.height; y++) {
					if (map.map[x][y].type != 1) blockTexturesFile << map.map[x][y].texture << " "; // Write blocks
					else blockTexturesFile << 0 << " "; // Do not write if player
				}

				blockTexturesFile << "\n"; // Start a new line for the next row
			}

			blockTexturesFile.close();
		}

		// Save solidity
		if (blockSolidsFile.is_open()) {
			for (int x = 0; x < map.width; x++) {
				for (int y = 0; y < map.height; y++) {
					blockSolidsFile << map.map[x][y].isSolid << " "; // Write blocks
				}

				blockSolidsFile << "\n"; // Start a new line for the next row
			}

			blockSolidsFile.close();
		}
	}

	void LoadMap(game::Map &map) {
		/**
		 * @brief This function loads saved map
		 * @param map Game map
		 */
		
		// Open file
		std::ifstream blockTypesFile("save/maptypes.msave");
		std::ifstream blockColorsFile("save/mapcolors.msave");
		std::ifstream blockTexturesFile("save/maptextures.msave");

		std::ifstream blockSolidsFile("save/mapsolids.msave");

		// Load blocktypes
		if (blockTypesFile.is_open()) {
			for (int x = 0; x < map.width; x++) {
				for (int y = 0; y < map.height; y++) {
					blockTypesFile >> map.map[x][y].type;
				}
			}
			blockTypesFile.close();
		}
		// Load colors
		if (blockColorsFile.is_open()) {
			for (int x = 0; x < map.width; x++) {
				for (int y = 0; y < map.height; y++) {
					blockColorsFile >> map.map[x][y].color;
				}
			}
			blockColorsFile.close();
		}
		// Load textures
		if (blockTexturesFile.is_open()) {
			for (int x = 0; x < map.width; x++) {
				for (int y = 0; y < map.height; y++) {
					blockTexturesFile >> map.map[x][y].texture;
				}
			}
			blockTexturesFile.close();
		}

		// Load solids
		if (blockSolidsFile.is_open()) {
			for (int x = 0; x < map.width; x++) {
				for (int y = 0; y < map.height; y++) {
					blockSolidsFile >> map.map[x][y].isSolid;
				}
			}
			blockSolidsFile.close();
		}
	}
}
