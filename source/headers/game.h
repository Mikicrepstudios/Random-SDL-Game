#pragma once

#include "mf/core.h"

#include "settings.h"
#include "textures.h"

/**
 * @brief Namespace that contains all game core functions
 */
namespace game {
	// Essential stuff
	void CameraControl(core::MF_Window &window, game::Settings settings, game::Camera &cam);

	// Map related stuff
	void RenderMap(core::MF_Window &window, game::Settings settings, game::Map map, game::Camera cam, textures::BlockTextures blockTextures[32]);
	void ClearMap(game::Map &map);
}