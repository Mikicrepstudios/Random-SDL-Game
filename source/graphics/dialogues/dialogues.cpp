#include "mf/core.h"

#include "settings.h"
#include "dialogues.h"

namespace dialogues {
    void CallDialogue(core::MF_Window &window, game::Game &game, rects dialoguesRects) {
        /**
         * @brief This function is used to draw dialogues, it is always ran in main() and will draw dialogue based on dialogueId
         */

        if(game.settings.dialogue) // Check if dialogue is enabled
        switch(game.settings.dialogueId) {
            case 1:
                // Confirm exit game dialogue
                ConfirmExitDialogue(window, game, dialoguesRects);
                break;
            case 2:
                // Confirm clear map dialogue
                ClearMapDialogue(window, game, dialoguesRects);
                break;
            case 3:
                // Enter save path dialogue
                SaveDialogue(window, game, dialoguesRects);
                break;
        }
    }
}