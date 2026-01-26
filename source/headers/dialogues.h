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

    // Functions needed for CallDialogue()
    void ClearMapDialogue(core::MF_Window window, game::Settings &settings, game::Map &map, rects dialoguesRects);
    void ConfirmExitDialogue(core::MF_Window window, game::Game &game, game::Settings &settings, rects dialoguesRects);
    void SaveDialogue(core::MF_Window &window, game::Settings &settings, game::Game &game, rects dialoguesRects);


    //int ConfirmDialogueEvent(core::MF_Window &window, rects dialoguesRects); // TODO: Destroy
    //bool ConfirmDialogue(core::MF_Window &window, game::Settings settings, rects dialoguesRects); // TODO: Annihilate

    void CallDialogue(core::MF_Window &window, game::Game &game, game::Settings &settings, game::Map &map, rects dialoguesRects); // Only called once in main()
}