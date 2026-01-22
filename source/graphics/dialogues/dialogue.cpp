#include "mf/core.h"

#include "settings.h"
#include "dialogues.h"

namespace dialogues {
    void CallDialogue(core::MF_Window window, game::Game &game, game::Settings &settings, rects dialoguesRects) {
        /**
         * @brief This function is used to draw dialogues, it is always ran in main() and will draw dialogue based on dialogueId
         */

        if(settings.dialogue) // Check if dialogue is enabled
        switch(settings.dialogueId) {
            case 1:
                // Confirm exit game dialogue
                ConfirmExitDialogue(window, game, settings, dialoguesRects);
                break;
        }

    }
}