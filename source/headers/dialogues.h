#pragma once

#include "mf/core.h"

#include "settings.h"

/**
 * @brief Namespace that contains all game dialogues
 */
namespace dialogues {
    struct rects {
        SDL_Rect backgroundRect = {};
        SDL_Rect titleRect = {};
        SDL_Rect descRect = {};
        SDL_Rect noRect = {};
        SDL_Rect yesRect = {};
        SDL_Rect otherYesRect = {};
    };

    rects InitRects(core::MF_Window &window);

    int ConfirmDialogueEvent(core::MF_Window &window, rects dialoguesRects);
    bool ConfirmDialogue(core::MF_Window &window, game::Settings settings, rects dialoguesRects);
}