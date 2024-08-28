#pragma once

#include "presets.h"
#include "settings.h"

namespace player {
	// Hud
	extern void GameInfo(SDL_Renderer* renderer, game::SDL_Settings sdlSettings, game::Camera camera, game::Player player, SDL_Color color);

	// Events
	extern void PlayerMovement(SDL_Event event, game::Map &map, game::Player &player);
	
	extern int ColorPickerEvent(game::SDL_Settings sdlSettings, game::Settings &settings);
}
