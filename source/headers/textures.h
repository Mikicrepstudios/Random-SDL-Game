#pragma once

#include "SDL2/SDL.h"
#include <SDL_image.h>

#include "settings.h"

namespace textures {
    struct BlockTextures {
        SDL_Texture* texture = {};
    };

    extern void initBlocks(game::SDL_Settings &sdlSettings, BlockTextures blockTextures[32]);

    // Pickers
    extern int PickerEvent(game::SDL_Settings sdlSettings, game::Settings &settings);
    extern void PickerOverlay(game::SDL_Settings sdlSettings, textures::BlockTextures blockTextures[32]);
}