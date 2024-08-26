#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "addional.h"
#include "dialogues.h"

namespace dialogues {
	int ConfirmDialogueEvent(SDL_Event event, game::SDL_Settings sdlSettings, rects dialoguesRects) {
        int mouseX = sdlSettings.mouseX;
        int mouseY = sdlSettings.mouseY;
        
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if      (mouseX >= dialoguesRects.yesRect.x && mouseX <= dialoguesRects.yesRect.x + dialoguesRects.yesRect.w &&
                    mouseY >= dialoguesRects.yesRect.y && mouseY <= dialoguesRects.yesRect.y + dialoguesRects.yesRect.h)
                    return 2;
            else if (mouseX >= dialoguesRects.otherYesRect.x && mouseX <= dialoguesRects.otherYesRect.x + dialoguesRects.otherYesRect.w &&
                    mouseY >= dialoguesRects.otherYesRect.y && mouseY <= dialoguesRects.otherYesRect.y + dialoguesRects.otherYesRect.h)
                    return 3; // Save and exit
        }

        return 0;
	}

    bool ConfirmDialogue(game::SDL_Settings sdlSettings, game::Settings settings, rects dialoguesRects) {
        SDL_Color textColor = {255, 255, 255};
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
        draw::DrawRect(sdlSettings.renderer, dialoguesRects.backgroundRect, 2);
        draw::DrawText(sdlSettings.renderer, sdlSettings.font, dialoguesRects.titleRect, titleText, textColor);

        // Draw buttons
        draw::DrawButton(sdlSettings.renderer, dialoguesRects.noRect, 27, sdlSettings.mouseX, sdlSettings.mouseY);
        draw::DrawButton(sdlSettings.renderer, dialoguesRects.yesRect, 13, sdlSettings.mouseX, sdlSettings.mouseY);

        // Give buttons text

        if(settings.dialogueId != 1) {
            draw::DrawText(sdlSettings.renderer, sdlSettings.font, dialoguesRects.noRect, "No", textColor);
            draw::DrawText(sdlSettings.renderer, sdlSettings.font, dialoguesRects.yesRect, "Yes", textColor);
            draw::DrawText(sdlSettings.renderer, sdlSettings.font, dialoguesRects.descRect, descText, textColor);
        }
        else {
            // Draw addional button for saving
            draw::DrawButton(sdlSettings.renderer, dialoguesRects.otherYesRect, 27, sdlSettings.mouseX, sdlSettings.mouseY);

            draw::DrawText(sdlSettings.renderer, sdlSettings.font, dialoguesRects.noRect, "Cancel", textColor);
            draw::DrawText(sdlSettings.renderer, sdlSettings.font, dialoguesRects.otherYesRect, "Exit without saving", textColor);
            draw::DrawText(sdlSettings.renderer, sdlSettings.font, dialoguesRects.yesRect, "Exit with saving", textColor);
        }

        if (sdlSettings.mouseX >= dialoguesRects.noRect.x && sdlSettings.mouseX <= dialoguesRects.noRect.x + dialoguesRects.noRect.w &&
				sdlSettings.mouseY >= dialoguesRects.noRect.y && sdlSettings.mouseY <= dialoguesRects.noRect.y + dialoguesRects.noRect.h)
                return true;

        return false;
    }
}