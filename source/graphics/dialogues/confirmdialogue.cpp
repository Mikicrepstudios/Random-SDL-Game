#include "SDL.h"
#include "SDL_ttf.h"

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"
#include "mf/logic.h"

#include "dialogues.h"

namespace dialogues {
	int ConfirmDialogueEvent(core::MF_Window &window, rects dialoguesRects) {
        /**
         * @brief This function checks when user presses confirm button in dialogue
         * @param window Game window
         * @param dialoguesRects All dialgoue rects
         */

        if (window.event.type == SDL_MOUSEBUTTONDOWN) {
            if(logic::IsMouseTouching(window.mouse, dialoguesRects.yesRect)) return 2;
            else if(logic::IsMouseTouching(window.mouse, dialoguesRects.otherYesRect)) return 3; // Save and exit
        }

        return 0;
	}

    bool ConfirmDialogue(core::MF_Window &window, game::Settings settings, rects dialoguesRects) {
        /**
         * @brief This function draws dialgoue and closes when cancel is hovered
         * @param window Game window
         * @param settings Game settings
         * @param dialoguesRects All dialgoue rects
         */

        const char* titleText = "";
        const char* descText = "You will loose any unsaved progress.";

        switch (settings.dialogueId) {
            case 1:
                titleText = "Are you sure you want to quit game?";
                break;
            case 2:
                titleText = "Are you sure you want to clear map?";
                break;
        }

        // Draw dialogue
        draw::DrawRect(window.renderer, dialoguesRects.backgroundRect, colors::gray);
        draw::DrawText(window.renderer, window.font, dialoguesRects.titleRect, titleText, colors::white);

        // Draw buttons
        draw::DrawButton(window.renderer, dialoguesRects.noRect, colors::red, window.mouse);
        draw::DrawButton(window.renderer, dialoguesRects.yesRect, colors::lightgreen, window.mouse);

        // Give buttons text

        if(settings.dialogueId != 1) {
            draw::DrawText(window.renderer, window.font, dialoguesRects.noRect, "No", colors::white);
            draw::DrawText(window.renderer, window.font, dialoguesRects.yesRect, "Yes", colors::white);
            draw::DrawText(window.renderer, window.font, dialoguesRects.descRect, descText, colors::white);
        }
        else {
            // Draw addional button for saving
            draw::DrawButton(window.renderer, dialoguesRects.otherYesRect, colors::red, window.mouse);

            draw::DrawText(window.renderer, window.font, dialoguesRects.noRect, "Cancel", colors::white);
            draw::DrawText(window.renderer, window.font, dialoguesRects.otherYesRect, "Exit without saving", colors::white);
            draw::DrawText(window.renderer, window.font, dialoguesRects.yesRect, "Exit with saving", colors::white);
        }

        if(logic::IsMouseTouching(window.mouse, dialoguesRects.noRect)) return true;

        return false;
    }
}