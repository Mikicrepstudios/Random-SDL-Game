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

    rects initRects(game::SDL_Settings sdlSettings);

    extern int confirmDialogueEvent(SDL_Event event, int mouseX, int mouseY, int width, int height, rects dialoguesRects);
    extern int confirmDialogue(SDL_Renderer* renderer, TTF_Font* font, int width, int height, int mouseX, int mouseY, int textid, rects dialoguesRects);
}