#pragma once

#include "settings.h"

namespace events {
	extern void Camera(game::SDL_Settings sdlSettings, game::Settings settings, game::Camera &cam);
}

namespace game {
	extern void RenderMap(game::SDL_Settings sdlSettings, game::Map map, game::Camera cam);
}

namespace gamemap {
	extern void ClearMap(game::Map &map);
}
