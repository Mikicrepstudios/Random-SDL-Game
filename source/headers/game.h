#pragma once

#include "mf/core.h"

#include "settings.h"
#include "textures.h"


/**
 * @brief Namespace that contains all game core functions
 */
namespace game {
	// Structs
	struct Preset {
		int blockColor = 1; // Write here to update
		int textureId = 0; // Write here to update
	};

	// Core
	void MainMenu(core::MF_Window &window, bool &running);
    void UpdateVars(game::Settings &settings, game::Player &player, game::Camera &cam, game::Preset preset[10]);

	// High level game stuff
    void MouseEvent(core::MF_Window &window, game::Game game, game::Settings &settings, game::Map &map, game::Camera &cam, game::Preset preset[10]);
    void MouseOverlay(core::MF_Window &window, game::Game game, game::Settings settings, game::Map map, game::Camera cam);
	
	// Medium level game stuff
	void CameraControls(core::MF_Window &window, game::Settings settings, game::Map map, game::Camera &cam);
	void PlayerMovement(SDL_Event event, game::Map &map, game::Player &player);

	// Low level game stuff
	void PresetChooser(SDL_Event event, int &preset);

	// Map related stuff
	void RenderMap(core::MF_Window &window, game::Settings settings, game::Map map, game::Camera cam, textures::BlockTextures blockTextures[32]);
	void ClearMap(game::Map &map);
}