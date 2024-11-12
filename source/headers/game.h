#pragma once

#include "mf/core.h"

#include "settings.h"
#include "textures.h"

namespace events {
	void Camera(core::MF_Window &window, game::Settings settings, game::Camera &cam);
}

namespace game {
	void RenderMap(core::MF_Window &window, game::Settings settings, game::Map map, game::Camera cam, textures::BlockTextures blockTextures[32]);
}

namespace gamemap {
	void ClearMap(game::Map &map);
}
