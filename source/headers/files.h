#pragma once

#include "block.h"
#include "game.h"
#include "settings.h"

/**
 * @brief Namespace that contains all functions that are related to managing files on disk
 */
namespace files {
	void LoadGame_v1(game::Game &game, std::ifstream& file);
	void LoadLegacyMap(game::Game &game); // Unused, but kept for legacy support

	void SaveGame(const game::Game &game);
	void LoadGame(game::Game &game);
}	
