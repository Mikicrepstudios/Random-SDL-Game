#include "mf/core.h"

#include "dialogues.h"

namespace dialogues {
    rects InitRects(core::MF_Window &window) {
        /**
         * @brief This function initilizes dialogue rects
         * @param window Game window
         */
        
        rects rects = {};
        // Name                    X Offset                     Y Offset                    Width Height
        rects.backgroundRect = {window.width / 2 - 300, window.height / 2 - 100, 600, 200};
        rects.titleRect      = {window.width / 2 - 300, window.height / 2 - 100, 600, 50 };
        rects.descRect       = {window.width / 2 - 275, window.height / 2 - 40,  550, 25 };
        rects.noRect         = {window.width / 2 - 275, window.height / 2 + 25,  200, 50 };
        rects.yesRect        = {window.width / 2 + 75,  window.height / 2 + 25,  200, 50 };
        rects.otherYesRect   = {window.width / 2 + 75,  window.height / 2 - 40,  200, 50 };

        return rects;
    }
}