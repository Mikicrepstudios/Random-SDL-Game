#pragma once

#include "block.h"
#include "game.h"
#include "settings.h"

/**
 * @brief Namespace that contains all functions that are related to managing files on disk
 */
namespace files {
	void LoadGame_v1(game::Map& map, game::Settings& settings, game::Player& player, game::Camera& cam, std::ifstream& file);

	void SaveGame(const game::Map& map, const game::Settings& settings, const game::Player& player, const game::Camera& cam, const std::string& slotName);
	void LoadGame(game::Map& map, game::Settings& settings, game::Player& player, game::Camera& cam, const std::string& saveName);
}	
