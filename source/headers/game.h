#pragma once

#include "mf/core.h"

#include "presets.h"
#include "settings.h"
#include "textures.h"

/**
 * @brief Namespace that contains all game core functions
 */
namespace game {
	// Core
    void UpdateVars(game::Settings &settings, game::Player &player, game::Camera &cam, game::Preset preset[10]);

	// Game stuff
	void CameraControl(core::MF_Window &window, game::Settings settings, game::Camera &cam);
	void PlayerMovement(SDL_Event event, game::Map &map, game::Player &player);

	// Map related stuff
	void RenderMap(core::MF_Window &window, game::Settings settings, game::Map map, game::Camera cam, textures::BlockTextures blockTextures[32]);
	void ClearMap(game::Map &map);
}