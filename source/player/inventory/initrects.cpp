#include "inventory.h"
#include "settings.h"

namespace inventory {
    rects InitRects(game::SDL_Settings sdlSettings) {
		// widthFactor and heightFactor // TODO //
		/*float wFactor = sdlSettings.width / 1280;
		float hFactor = sdlSettings.height / 800;*/

		rects rects = {};
		// Name                     X Offset                      Y Offset                 Width Height
		rects.colorRect           = {50,                          50,                       80,  80 };
		rects.colorRectb          = {45,                          45,                       90,  90 };
		rects.colorTextRect       = {50,                          140,                      80,  40 };

		rects.bgColorRect         = {sdlSettings.width - 130,     50,                       80,  80 };
		rects.bgColorRectb        = {sdlSettings.width - 135,     45,                       90,  90 };
		rects.bgColorTextRect     = {sdlSettings.width - 130,     140,                      80,  40 };

		rects.playerColorRect     = {50,                          200,                      80,  80 };
		rects.playerColorRectb    = {45,                          195,                      90,  90 };
		rects.playerColorTextRect = {50,                          290,                      80,  40 };

		rects.previewRect         = {sdlSettings.width / 2 - 50,  50,                       100, 100};
		rects.previewRectb        = {sdlSettings.width / 2 - 55,  45,                       110, 110};
		rects.previewTextRect     = {sdlSettings.width / 2 - 50,  160,                      100, 40 };

        rects.solidRect           = {50,                          sdlSettings.height - 355, 80,  80 };
        rects.solidRectb          = {45,                          sdlSettings.height - 360, 90,  90 };
        rects.solidTextRect       = {50,                          sdlSettings.height - 260, 80,  40 };

		rects.bgGameplay          = {45,                          sdlSettings.height - 215, 420, 170};
		rects.gameplayTextRect    = {150,                         sdlSettings.height - 210, 200, 50 };
		rects.camTpRect           = {50,                          sdlSettings.height - 160, 200, 50 };
		rects.playerTpRect        = {50,                          sdlSettings.height - 100, 200, 50 };
		rects.bgGame              = {sdlSettings.width - 465,     sdlSettings.height - 215, 420, 170};
		rects.gameTextRect        = {sdlSettings.width - 360,     sdlSettings.height - 210, 200, 50 };
		rects.saveRect            = {sdlSettings.width - 460,     sdlSettings.height - 160, 200, 50 };
		rects.loadRect            = {sdlSettings.width - 460,     sdlSettings.height - 100, 200, 50 };
		rects.gameInfoRect        = {sdlSettings.width - 250,     sdlSettings.height - 160, 200, 50 };
		rects.exitRect            = {sdlSettings.width - 250,     sdlSettings.height - 100, 200, 50 };
		rects.presetRect          = {sdlSettings.width / 2 - 85,  sdlSettings.height - 160, 170, 115};
		rects.presetTextRect      = {sdlSettings.width / 2 - 40,  sdlSettings.height - 160, 80,  115};
		rects.presetTitleRect     = {sdlSettings.width / 2 - 115, sdlSettings.height - 215, 230, 50 };
		return rects;
	}
}