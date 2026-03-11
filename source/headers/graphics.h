#pragma once

#include "settings.h"
#include "textures.h"

namespace graphics {
void DrawBlock(core::MF_Window &window, game::Game::Database::Items &block,
               SDL_Rect rect, textures::BlockTextures blockTextures[32]);
}