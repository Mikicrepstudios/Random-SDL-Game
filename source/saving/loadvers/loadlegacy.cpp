#include <fstream>

#include "settings.h"

namespace files {
    void LoadLegacyMap(game::Game &game) {
		/**
		 * @brief This function is used for loading old legacy maps, for 2.0.1 but maybe supports older versions, it will never be included in loading code, but incase you want to try converting map then you can try manually using this
		 * @param map Game map
		 */

		auto& map = game.map;
		
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