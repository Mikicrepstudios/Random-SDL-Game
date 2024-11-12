#pragma once

#include "SDL2/SDL.h"
#include <SDL_image.h>

#include "mf/core.h"

#include "settings.h"

namespace textures {
    struct BlockTextures {
        SDL_Texture* texture = {};
    };

    extern void initBlocks(game::SDL_Settings &sdlSettings, BlockTextures blockTextures[32]);

    // Pickers
    int PickerEvent(core::MF_Window &window, game::Settings &settings);
    void PickerOverlay(core::MF_Window &window, textures::BlockTextures blockTextures[32]);
}