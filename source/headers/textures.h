#pragma once

#include "SDL.h"
#include "SDL_image.h"

#include "mf/core.h"

#include "settings.h"
#include "textures.h"

/**
 * @brief Namespace that contains all stuff related to game textures
 */
namespace textures {
    struct BlockTextures {
        SDL_Texture* texture = {};
    };

    void initBlocks(core::MF_Window &window, BlockTextures blockTextures[32]);

    // Pickers
    int PickerEvent(core::MF_Window &window, game::Game &game);
    void PickerOverlay(core::MF_Window &window, textures::BlockTextures blockTextures[32]);
}