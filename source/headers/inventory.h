#ifndef INVENTORY_H
#define INVENTORY_H

#include <SDL2/SDL.h>

namespace inventory {
    struct rects {
        SDL_Rect colorRect = {};
        SDL_Rect colorRectb = {};
        SDL_Rect colorTextRect = {};
        SDL_Rect bgColorRect = {};
        SDL_Rect bgColorRectb = {};
        SDL_Rect bgColorTextRect = {};
        SDL_Rect playerColorRect = {};
        SDL_Rect playerColorRectb = {};
        SDL_Rect playerColorTextRect = {};
        SDL_Rect previewRect = {};
        SDL_Rect previewRectb = {};
        SDL_Rect previewTextRect = {};
        SDL_Rect bgGameplay = {};
        SDL_Rect gameplayTextRect = {};
        SDL_Rect camTpRect = {};
        SDL_Rect playerTpRect = {};
        SDL_Rect bgGame = {};
        SDL_Rect gameTextRect = {};
        SDL_Rect saveRect = {};
        SDL_Rect loadRect = {};
        SDL_Rect gameInfoRect = {};
        SDL_Rect exitRect = {};
        SDL_Rect presetRect = {};
        SDL_Rect presetTextRect = {};
        SDL_Rect selCLPresetRect = {};
        SDL_Rect selCRPresetRect = {};
        SDL_Rect presetTitleRect = {};
    };

    rects InitRects(game::SDL_Settings sdlSettings);
}

#endif