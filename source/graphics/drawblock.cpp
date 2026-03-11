#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"

#include "settings.h"
#include "textures.h"

namespace graphics {
void DrawBlock(core::MF_Window &window, game::Game::Database::Items &block,
               SDL_Rect rect, textures::BlockTextures blockTextures[32]) {
  draw::DrawRect(window.renderer, rect, colors::colorID[block.color - 1]);

  if (block.texture != 0)
    draw::DrawTextureRect(window.renderer, rect,
                          blockTextures[block.texture].texture);
}
} // namespace graphics