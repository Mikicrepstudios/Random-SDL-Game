#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "addional.h"

namespace dialogues {
	int confirmDialogueEvent(SDL_Event event, int mouseX, int mouseY, int width, int height) {
        SDL_Rect confirmRect = {width / 2 + 75, height / 2 + 25, 200, 50};
        SDL_Rect forceExitRect = {width / 2 + 75, height / 2 - 40, 200, 50};
        
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (mouseX >= confirmRect.x && mouseX <= confirmRect.x + confirmRect.w &&
                    mouseY >= confirmRect.y && mouseY <= confirmRect.y + confirmRect.h)
                    return 2;
            else if (mouseX >= forceExitRect.x && mouseX <= forceExitRect.x + forceExitRect.w &&
                    mouseY >= forceExitRect.y && mouseY <= forceExitRect.y + forceExitRect.h)
                    return 3; // Save and exit
        }

        return 0;
	}

    int confirmDialogue(SDL_Renderer* renderer, TTF_Font* font, int width, int height, int mouseX, int mouseY, int textid) {
        SDL_Rect backgroundRect = {width / 2 - 300, height / 2 - 100, 600, 200};
        SDL_Rect titleRect = {width / 2 - 300, height / 2 - 100, 600, 50};
        SDL_Rect descRect = {width / 2 - 275, height / 2 - 40, 550, 25};
        SDL_Rect noRect = {width / 2 - 275, height / 2 + 25, 200, 50};
        SDL_Rect confirmRect = {width / 2 + 75, height / 2 + 25, 200, 50};
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
        draw::DrawRect(renderer, backgroundRect, 2);
        draw::DrawText(renderer, font, titleRect, titleText, textColor);

        // Draw buttons
        draw::DrawButton(renderer, noRect, 27, mouseX, mouseY);
        draw::DrawButton(renderer, confirmRect, 13, mouseX, mouseY);

        // Give buttons text

        if(textid != 1) {
            draw::DrawText(renderer, font, noRect, "No", textColor);
            draw::DrawText(renderer, font, confirmRect, "Yes", textColor);
            draw::DrawText(renderer, font, descRect, descText, textColor);
        }
        else {
            // Draw addional button for saving
            SDL_Rect forceExitRect = {width / 2 + 75, height / 2 - 40, 200, 50};
            draw::DrawButton(renderer, forceExitRect, 27, mouseX, mouseY);

            draw::DrawText(renderer, font, noRect, "Cancel", textColor);
            draw::DrawText(renderer, font, forceExitRect, "Exit without saving", textColor);
            draw::DrawText(renderer, font, confirmRect, "Exit with saving", textColor);
        }

        if (mouseX >= noRect.x && mouseX <= noRect.x + noRect.w &&
				mouseY >= noRect.y && mouseY <= noRect.y + noRect.h)
                return 1;

        return 0;
    }
}