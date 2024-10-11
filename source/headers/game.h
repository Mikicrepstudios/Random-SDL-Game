#pragma once

#include "settings.h"
#include "textures.h"

namespace events {
	extern void Camera(game::SDL_Settings sdlSettings, game::Settings settings, game::Camera &cam);
}

namespace game {
	extern void RenderMap(game::SDL_Settings sdlSettings, game::Settings settings, game::Map map, game::Camera cam, textures::BlockTextures blockTextures[32]);
}

namespace gamemap {
	extern void ClearMap(game::Map &map);
}
