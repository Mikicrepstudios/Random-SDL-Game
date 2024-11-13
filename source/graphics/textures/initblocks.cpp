#include "SDL2/SDL.h"
#include <SDL_image.h>

#include "mf/core.h"

#include "textures.h"

namespace textures {
    void initBlocks(core::MF_Window &window, BlockTextures blockTextures[32]) {
        blockTextures[0].texture = nullptr;
        blockTextures[1].texture = core::LoadImg(window.renderer, "assets/textures/blocks/placeholder.png");
        blockTextures[2].texture = core::LoadImg(window.renderer, "assets/textures/blocks/bricks.png");
        blockTextures[3].texture = core::LoadImg(window.renderer, "assets/textures/blocks/stone.png");
        blockTextures[4].texture = core::LoadImg(window.renderer, "assets/textures/blocks/wood.png");
        blockTextures[5].texture = core::LoadImg(window.renderer, "assets/textures/blocks/logs.png");
        blockTextures[6].texture = core::LoadImg(window.renderer, "assets/textures/blocks/grass.png");
        blockTextures[7].texture = core::LoadImg(window.renderer, "assets/textures/blocks/dirt.png");

        blockTextures[8].texture = core::LoadImg(window.renderer, "assets/textures/blocks/sand.png");
        blockTextures[9].texture = core::LoadImg(window.renderer, "assets/textures/blocks/ore.png");
        blockTextures[10].texture = core::LoadImg(window.renderer, "assets/textures/blocks/glass.png");
        blockTextures[11].texture = core::LoadImg(window.renderer, "assets/textures/blocks/water.png");
        blockTextures[12].texture = core::LoadImg(window.renderer, "assets/textures/blocks/lava.png");
        blockTextures[13].texture = core::LoadImg(window.renderer, "assets/textures/blocks/wool.png");
        blockTextures[14].texture = core::LoadImg(window.renderer, "assets/textures/blocks/leaves.png");
        blockTextures[15].texture = core::LoadImg(window.renderer, "assets/textures/blocks/placeholder.png"); //TODO CACTUS
    }
}