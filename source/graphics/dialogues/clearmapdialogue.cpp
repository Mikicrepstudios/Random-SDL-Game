#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"
#include "mf/logic.h"

#include "dialogues.h"
#include "game.h"

namespace dialogues {
    void ClearMapDialogue(core::MF_Window window, game::Game &game, rects dialoguesRects) {
        const char* titleText = "Are you sure you want to clear map?";
        const char* descText = "You will loose any unsaved progress.";
        
        // Draw dialogue
        draw::DrawRect(window.renderer, dialoguesRects.backgroundRect, colors::gray);
        draw::DrawText(window.renderer, window.font, dialoguesRects.titleRect, titleText, colors::white);
        draw::DrawText(window.renderer, window.font, dialoguesRects.descRect, descText, colors::white);

        // Draw buttons
        draw::DrawButton(window.renderer, dialoguesRects.noRect, colors::red, window.mouse);
        draw::DrawButton(window.renderer, dialoguesRects.yesRect, colors::lightgreen, window.mouse);

        // Draw text
        draw::DrawText(window.renderer, window.font, dialoguesRects.noRect, "No", colors::white);
        draw::DrawText(window.renderer, window.font, dialoguesRects.yesRect, "Yes", colors::white);
        
        // Detect mouse clicks
        if(window.mouse.isPressed) {
            if(logic::IsMouseTouching(window.mouse, dialoguesRects.noRect)) game.settings.dialogue = false;
            if(logic::IsMouseTouching(window.mouse, dialoguesRects.yesRect)) {
                game::ClearMap(game);
                game.settings.dialogue = false;
            }
        }
    }
}