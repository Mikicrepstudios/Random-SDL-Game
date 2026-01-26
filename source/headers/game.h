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
	void MainMenu(core::MF_Window &window, game::Game &game, bool &running);
    void UpdateVars(game::Game &game, game::Preset preset[10]);

	// High level game stuff
    void MouseEvent(core::MF_Window &window, game::Game &game, game::Preset preset[10]);
    void MouseOverlay(core::MF_Window &window, game::Game &game);
	
	// Medium level game stuff
	void CameraControls(core::MF_Window &window, game::Game &game);
	void PlayerMovement(SDL_Event event, game::Game &game);

	// Low level game stuff
	void PresetChooser(SDL_Event event, int &preset);

	// Map related stuff
	void RenderMap(core::MF_Window &window, game::Game &game, textures::BlockTextures blockTextures[32]);
	void ClearMap(game::Game &game);
}