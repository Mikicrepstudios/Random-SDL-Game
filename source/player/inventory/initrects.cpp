#include "mf/core.h"

#include "inventory.h"

namespace inventory {
    rects InitRects(core::MF_Window &window) {
		// widthFactor and heightFactor // TODO //
		/*float wFactor = sdlSettings.width / 1280;
		float hFactor = sdlSettings.height / 800;*/

		rects rects = {};
		// Name                     X Offset                      Y Offset                 Width Height
		rects.colorRect           = {50,                          50,                       80,  80 };
		rects.colorRectb          = {45,                          45,                       90,  90 };
		rects.colorTextRect       = {50,                          140,                      80,  40 };

		rects.bgColorRect         = {155,                         50,                       80,  80 };
		rects.bgColorRectb        = {150,                         45,                       90,  90 };
		rects.bgColorTextRect     = {155,                         140,                      80,  40 };

		rects.playerColorRect     = {50,                          200,                      80,  80 };
		rects.playerColorRectb    = {45,                          195,                      90,  90 };
		rects.playerColorTextRect = {50,                          290,                      80,  40 };

		// TODO SET THEESE WHEN NEEDED
		rects.textureColorRect    = {window.width - 130,     50,                       80,  80 };
		rects.textureColorRectb   = {window.width - 135,     45,                       90,  90 };
		rects.textureColorTextRect= {window.width - 130,     140,                      80,  40 };

		rects.previewRect         = {window.width / 2 - 50,  50,                       100, 100};
		rects.previewRectb        = {window.width / 2 - 55,  45,                       110, 110};
		rects.previewTextRect     = {window.width / 2 - 50,  160,                      100, 40 };

		rects.textureIdRect       = {window.width - 130,     50,                       80,  80 };
		rects.textureIdRectb      = {window.width - 135,     45,                       90,  90 };
		rects.textureIdTextRect   = {window.width - 130,     140,                      80,  40 };

        rects.solidRect           = {50,                          window.height - 355, 80,  80 };
        rects.solidRectb          = {45,                          window.height - 360, 90,  90 };
        rects.solidTextRect       = {50,                          window.height - 260, 80,  40 };

		rects.bgGameplay          = {45,                          window.height - 215, 420, 170};
		rects.gameplayTextRect    = {150,                         window.height - 210, 200, 50 };
		rects.camTpRect           = {50,                          window.height - 160, 200, 50 };
		rects.playerTpRect        = {50,                          window.height - 100, 200, 50 };
		rects.bgGame              = {window.width - 465,     window.height - 215, 420, 170};
		rects.gameTextRect        = {window.width - 360,     window.height - 210, 200, 50 };
		rects.saveRect            = {window.width - 460,     window.height - 160, 200, 50 };
		rects.loadRect            = {window.width - 460,     window.height - 100, 200, 50 };
		rects.gameInfoRect        = {window.width - 250,     window.height - 160, 200, 50 };
		rects.exitRect            = {window.width - 250,     window.height - 100, 200, 50 };
		rects.presetRect          = {window.width / 2 - 85,  window.height - 160, 170, 115};
		rects.presetTextRect      = {window.width / 2 - 40,  window.height - 160, 80,  115};
		rects.presetTitleRect     = {window.width / 2 - 115, window.height - 215, 230, 50 };
		return rects;
	}
}