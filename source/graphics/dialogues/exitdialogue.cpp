#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"
#include "mf/logic.h"

#include "dialogues.h"

namespace dialogues {
    void ConfirmExitDialogue(core::MF_Window window, game::Game &game, game::Settings &settings, rects dialoguesRects) {
        const char* titleText = "Are you sure you want to quit game?";

        // Draw dialogue
        draw::DrawRect(window.renderer, dialoguesRects.backgroundRect, colors::gray);
        draw::DrawText(window.renderer, window.font, dialoguesRects.titleRect, titleText, colors::white);

        // Draw buttons
        draw::DrawButton(window.renderer, dialoguesRects.noRect, colors::red, window.mouse);
        draw::DrawButton(window.renderer, dialoguesRects.yesRect, colors::lightgreen, window.mouse);
        draw::DrawButton(window.renderer, dialoguesRects.otherYesRect, colors::red, window.mouse);

        // Draw text
        draw::DrawText(window.renderer, window.font, dialoguesRects.noRect, "Cancel", colors::white);
        draw::DrawText(window.renderer, window.font, dialoguesRects.otherYesRect, "Exit without saving", colors::white);
        draw::DrawText(window.renderer, window.font, dialoguesRects.yesRect, "Exit with saving", colors::white);

        if(window.mouse.isDown) {
            if(logic::IsMouseTouching(window.mouse, dialoguesRects.noRect)) settings.dialogue = false;
            if(logic::IsMouseTouching(window.mouse, dialoguesRects.otherYesRect)) game.running = false;
            // TODO add Exit with saving call save function
        }
    }
}