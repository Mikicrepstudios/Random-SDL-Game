#pragma once

#include "presets.h"
#include "settings.h"

namespace player {
	// Hud
	extern void GameInfo(SDL_Renderer* renderer, game::SDL_Settings sdlSettings, game::Camera camera, game::Player player, SDL_Color color);

	// Events
	extern void PlayerMovement(SDL_Event event, game::Map &map, game::Player &player);

	extern void MouseEvent(SDL_Event event, game::SDL_Settings sdlSettings, game::Settings &settings, game::Map &map, game::Camera &camera, game::Preset preset[10]);

	extern void InventoryEvent(SDL_Event event, game::Settings &settings);

	extern void MouseInvChooser(SDL_Renderer* renderer, SDL_Event event, inventory::rects rects, game::SDL_Settings &sdlSettings, game::Settings &settings, game::Map &map, game::Player &player, game::Camera &camera, game::Cheats &cheats, game::Preset preset[10]);
	
	extern int ColorPickerEvent(game::SDL_Settings sdlSettings, game::Settings &settings);
}
