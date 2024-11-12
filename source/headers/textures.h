#pragma once

#include "SDL2/SDL.h"
#include <SDL_image.h>

#include "mf/core.h"

#include "textures.h"

namespace textures {
    struct BlockTextures {
        SDL_Texture* texture = {};
    };

    extern void initBlocks(core::MF_Window &window, BlockTextures blockTextures[32]);

    // Pickers
    int PickerEvent(core::MF_Window &window, game::Settings &settings);
    void PickerOverlay(core::MF_Window &window, textures::BlockTextures blockTextures[32]);
}