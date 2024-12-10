#pragma once

#include "mf/core.h"

#include "settings.h"
#include "textures.h"

/**
 * @brief Namespace that contains all game events
 */
namespace events {
	void Camera(core::MF_Window &window, game::Settings settings, game::Camera &cam);
}

/**
 * @brief Namespace that contains all game core functions
 */
namespace game {
	void RenderMap(core::MF_Window &window, game::Settings settings, game::Map map, game::Camera cam, textures::BlockTextures blockTextures[32]);
}

/**
 * @brief Namespace that contains all game map realted stuff
 */
namespace gamemap {
	void ClearMap(game::Map &map);
}
