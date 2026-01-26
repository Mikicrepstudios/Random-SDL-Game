#include <string>

#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"

#include "dialogues.h"
#include "settings.h"

namespace dialogues {
    void SaveDialogue(core::MF_Window &window, game::Game &game, rects dialoguesRects) {
        const char* titleText = "Enter save name";

        // Draw dialogue
        draw::DrawRect(window.renderer, dialoguesRects.backgroundRect, colors::gray);
        draw::DrawText(window.renderer, window.font, dialoguesRects.titleRect, titleText, colors::white);

        // Draw buttons
        draw::DrawInputRect(window, dialoguesRects.descRect, colors::darkgray, game.savePath);
        draw::DrawButton(window.renderer, dialoguesRects.noRect, colors::red, window.mouse);
        draw::DrawButton(window.renderer, dialoguesRects.yesRect, colors::lightgreen, window.mouse);

        // Draw text
        draw::DrawText(window.renderer, window.font, dialoguesRects.noRect, "Cancel", colors::white);
        draw::DrawText(window.renderer, window.font, dialoguesRects.yesRect, "Confirm", colors::white);
    }
}