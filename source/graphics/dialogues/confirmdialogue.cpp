#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "addional.h"
#include "dialogues.h"

namespace dialogues {
	int confirmDialogueEvent(SDL_Event event, int mouseX, int mouseY, int width, int height, rects dialoguesRects) {
        
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (mouseX >= dialoguesRects.yesRect.x && mouseX <= dialoguesRects.yesRect.x + dialoguesRects.yesRect.w &&
                    mouseY >= dialoguesRects.yesRect.y && mouseY <= dialoguesRects.yesRect.y + dialoguesRects.yesRect.h)
                    return 2;
            else if (mouseX >= dialoguesRects.otherYesRect.x && mouseX <= dialoguesRects.otherYesRect.x + dialoguesRects.otherYesRect.w &&
                    mouseY >= dialoguesRects.otherYesRect.y && mouseY <= dialoguesRects.otherYesRect.y + dialoguesRects.otherYesRect.h)
                    return 3; // Save and exit
        }

        return 0;
	}

    int confirmDialogue(SDL_Renderer* renderer, TTF_Font* font, int width, int height, int mouseX, int mouseY, int textid, rects dialoguesRects) {
        SDL_Color textColor = {255, 255, 255};
        const char* titleText = "";
        const char* descText = "You will loose any unsaved progress.";

        switch (textid) {
            case 1:
                titleText = "Are you sure you want to quit game?";
                break;
            case 2:
                titleText = "Are you sure you want to clear map?";
                break;
        }

        // Draw dialogue
        draw::DrawRect(renderer, dialoguesRects.backgroundRect, 2);
        draw::DrawText(renderer, font, dialoguesRects.titleRect, titleText, textColor);

        // Draw buttons
        draw::DrawButton(renderer, dialoguesRects.noRect, 27, mouseX, mouseY);
        draw::DrawButton(renderer, dialoguesRects.yesRect, 13, mouseX, mouseY);

        // Give buttons text

        if(textid != 1) {
            draw::DrawText(renderer, font, dialoguesRects.noRect, "No", textColor);
            draw::DrawText(renderer, font, dialoguesRects.yesRect, "Yes", textColor);
            draw::DrawText(renderer, font, dialoguesRects.descRect, descText, textColor);
        }
        else {
            // Draw addional button for saving
            draw::DrawButton(renderer, dialoguesRects.otherYesRect, 27, mouseX, mouseY);

            draw::DrawText(renderer, font, dialoguesRects.noRect, "Cancel", textColor);
            draw::DrawText(renderer, font, dialoguesRects.otherYesRect, "Exit without saving", textColor);
            draw::DrawText(renderer, font, dialoguesRects.yesRect, "Exit with saving", textColor);
        }

        if (mouseX >= dialoguesRects.noRect.x && mouseX <= dialoguesRects.noRect.x + dialoguesRects.noRect.w &&
				mouseY >= dialoguesRects.noRect.y && mouseY <= dialoguesRects.noRect.y + dialoguesRects.noRect.h)
                return 1;

        return 0;
    }
}