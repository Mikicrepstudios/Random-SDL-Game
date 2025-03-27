#include "mf/core.h"
#include "mf/extra.h"

#include "dialogues.h"

namespace dialogues {
    rects InitRects(core::MF_Window &window) {
        /**
         * @brief This function initilizes dialogue rects
         * @param window Game window
         */

        // Variables used for scaling elements
		float wFactor = extra::castToFloat(window.width) / 1280;
		float hFactor = extra::castToFloat(window.height) / 800;
        
        rects rects = {};
        // Name                    X Offset                     Y Offset                    Width Height
        rects.backgroundRect = {extra::castToInt(window.width / 2 - 300 * wFactor), extra::castToInt(window.height / 2 - 100 * hFactor), extra::castToInt(600 * wFactor), extra::castToInt(200 * hFactor)};
        rects.titleRect      = {extra::castToInt(window.width / 2 - 300 * wFactor), extra::castToInt(window.height / 2 - 100 * hFactor), extra::castToInt(600 * wFactor), extra::castToInt(50 * hFactor)};
        rects.descRect       = {extra::castToInt(window.width / 2 - 275 * wFactor), extra::castToInt(window.height / 2 - 40 * hFactor),  extra::castToInt(550 * wFactor), extra::castToInt(25 * hFactor)};
        rects.noRect         = {extra::castToInt(window.width / 2 - 275 * wFactor), extra::castToInt(window.height / 2 + 25 * hFactor),  extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};
        rects.yesRect        = {extra::castToInt(window.width / 2 + 75 * wFactor),  extra::castToInt(window.height / 2 + 25 * hFactor),  extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};
        rects.otherYesRect   = {extra::castToInt(window.width / 2 + 75 * wFactor),  extra::castToInt(window.height / 2 - 40 * hFactor),  extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};

        /*rects.backgroundRect = {window.width / 2 - 300, window.height / 2 - 100, extra::castToInt(600 * wFactor), extra::castToInt(200 * hFactor)};
        rects.titleRect      = {window.width / 2 - 300, window.height / 2 - 100, extra::castToInt(600 * wFactor), extra::castToInt(50 * hFactor)};
        rects.descRect       = {window.width / 2 - 275, window.height / 2 - 40,  extra::castToInt(550 * wFactor), extra::castToInt(25 * hFactor)};
        rects.noRect         = {window.width / 2 - 275, window.height / 2 + 25,  extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};
        rects.yesRect        = {window.width / 2 + 75,  window.height / 2 + 25,  extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};
        rects.otherYesRect   = {window.width / 2 + 75,  window.height / 2 - 40,  extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};*/

        return rects;
    }
}