#include "dialogues.h"
#include "settings.h"

namespace dialogues {
    rects initRects(game::SDL_Settings sdlSettings) {
        rects rects = {};
        // Name                    X Offset                     Y Offset                    Width Height
        rects.backgroundRect = {sdlSettings.width / 2 - 300, sdlSettings.height / 2 - 100, 600, 200};
        rects.titleRect      = {sdlSettings.width / 2 - 300, sdlSettings.height / 2 - 100, 600, 50 };
        rects.descRect       = {sdlSettings.width / 2 - 275, sdlSettings.height / 2 - 40,  550, 25 };
        rects.noRect         = {sdlSettings.width / 2 - 275, sdlSettings.height / 2 + 25,  200, 50 };
        rects.yesRect        = {sdlSettings.width / 2 + 75,  sdlSettings.height / 2 + 25,  200, 50 };
        rects.otherYesRect   = {sdlSettings.width / 2 + 75,  sdlSettings.height / 2 - 40,  200, 50 };

        return rects;
    }
}