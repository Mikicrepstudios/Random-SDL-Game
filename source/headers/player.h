#pragma once

#include "presets.h"
#include "settings.h"

namespace player {
	extern void InitGameInfoTexts(game::SDL_Settings sdlSettings, game::Settings settings, game::Camera camera, game::Player player, std::string gameInfoTexts[16]);
	extern void GameInfo(game::SDL_Settings sdlSettings, SDL_Color color, std::string gameInfoTexts[16]);

	// Events
	extern void PlayerMovement(SDL_Event event, game::Map &map, game::Player &player);
}
