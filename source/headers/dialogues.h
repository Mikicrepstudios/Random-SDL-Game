#pragma once

#include "settings.h"

namespace dialogues {
    struct rects {
        SDL_Rect backgroundRect = {};
        SDL_Rect titleRect = {};
        SDL_Rect descRect = {};
        SDL_Rect noRect = {};
        SDL_Rect yesRect = {};
        SDL_Rect otherYesRect = {};
    };

    rects InitRects(game::SDL_Settings sdlSettings);

    extern int ConfirmDialogueEvent(SDL_Event event, game::SDL_Settings sdlSettings, rects dialoguesRects);
    extern bool ConfirmDialogue(game::SDL_Settings sdlSettings, game::Settings settings, rects dialoguesRects);
}