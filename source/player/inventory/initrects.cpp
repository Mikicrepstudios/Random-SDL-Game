#include <iostream>

#include "mf/core.h"

#include "inventory.h"

namespace inventory {
	MenuRects InitMenuRects(core::MF_Window &window) {
		/**
		 * @brief This function initilizes inventory rects
		 * @param window Game window
		 */

		// Variables used for scaling elements
		float wFactor = static_cast<float>(window.width) / 1280;
		float hFactor = static_cast<float>(window.height) / 800;
		
		MenuRects rects = {};

		rects.previewRect         = {window.width / 2 - 50, static_cast<int>(50 * hFactor), static_cast<int>(100 * wFactor), static_cast<int>(100 * hFactor)};
		rects.previewRectb        = {window.width / 2 - 55, static_cast<int>(45 * hFactor), static_cast<int>(110 * wFactor), static_cast<int>(110 * hFactor)};
		rects.previewTextRect     = {window.width / 2 - 50, static_cast<int>(160 * hFactor), static_cast<int>(100 * wFactor), static_cast<int>(40 * hFactor)};

		rects.bgGameplay          = {static_cast<int>(45 * wFactor), static_cast<int>(window.height - 215 * hFactor), static_cast<int>(420 * wFactor), static_cast<int>(170 * hFactor)};
		rects.gameplayTextRect    = {static_cast<int>(150 * wFactor), static_cast<int>(window.height - 210 * hFactor), static_cast<int>(200 * wFactor), static_cast<int>(50 * hFactor) };
		rects.bgGame              = {static_cast<int>(window.width - 465 * wFactor), static_cast<int>(window.height - 215 * hFactor), static_cast<int>(420 * wFactor), static_cast<int>(170 * hFactor)};
		rects.gameTextRect        = {static_cast<int>(window.width - 360 * wFactor), static_cast<int>(window.height - 210 * hFactor), static_cast<int>(200 * wFactor), static_cast<int>(50 * hFactor)};

		return rects;
	}

	ColorRects InitColorRects(core::MF_Window &window) {
		/**
		 * @brief This function initilizes inventory rects
		 * @param window Game window
		 */

		// Variables used for scaling elements
		float wFactor = static_cast<float>(window.width) / 1280;
		float hFactor = static_cast<float>(window.height) / 800;
		
		ColorRects rects = {};

		rects.colorRect           = {static_cast<int>(50 * wFactor), static_cast<int>(50 * hFactor), static_cast<int>(80 * wFactor), static_cast<int>(80 * hFactor)};
		rects.colorRectb          = {static_cast<int>(45 * wFactor), static_cast<int>(45 * hFactor), static_cast<int>(90 * wFactor), static_cast<int>(90 * hFactor)};
		rects.colorTextRect       = {static_cast<int>(50 * wFactor), static_cast<int>(140 * hFactor), static_cast<int>(80 * wFactor), static_cast<int>(40 * hFactor)};

		rects.bgColorRect         = {static_cast<int>(155 * wFactor), static_cast<int>(50 * hFactor), static_cast<int>(80 * wFactor), static_cast<int>(80 * hFactor)};
		rects.bgColorRectb        = {static_cast<int>(150 * wFactor), static_cast<int>(45 * hFactor), static_cast<int>(90 * wFactor), static_cast<int>(90 * hFactor)};
		rects.bgColorTextRect     = {static_cast<int>(155 * wFactor), static_cast<int>(140 * hFactor), static_cast<int>(80 * wFactor), static_cast<int>(40 * hFactor)};

		rects.playerColorRect     = {static_cast<int>(50 * wFactor), static_cast<int>(200 * hFactor), static_cast<int>(80 * wFactor), static_cast<int>(80 * hFactor)};
		rects.playerColorRectb    = {static_cast<int>(45 * wFactor), static_cast<int>(195 * hFactor), static_cast<int>(90 * wFactor), static_cast<int>(90 * hFactor)};
		rects.playerColorTextRect = {static_cast<int>(50 * wFactor), static_cast<int>(290 * hFactor), static_cast<int>(80 * wFactor), static_cast<int>(40 * hFactor)};

		return rects;
	}

	DecalRects InitDecalRects(core::MF_Window &window) {
		/**
		 * @brief This function initilizes inventory rects
		 * @param window Game window
		 */
		
		// Variables used for scaling elements
		float wFactor = static_cast<float>(window.width) / 1280;
		float hFactor = static_cast<float>(window.height) / 800;
		
		DecalRects rects = {};

		rects.textureIdRect       = {static_cast<int>(window.width - 130 * wFactor), static_cast<int>(50 * hFactor), static_cast<int>(80 * wFactor), static_cast<int>(80 * hFactor)};
		rects.textureIdRectb      = {static_cast<int>(window.width - 135 * wFactor), static_cast<int>(45 * hFactor), static_cast<int>(90 * wFactor), static_cast<int>(90 * hFactor)};
		rects.textureIdTextRect   = {static_cast<int>(window.width - 130 * wFactor), static_cast<int>(140 * hFactor), static_cast<int>(80 * wFactor), static_cast<int>(40 * hFactor)};

		return rects;
	}
	GameplayRects InitGameplayRects(core::MF_Window &window) {
		/**
		 * @brief This function initilizes inventory rects
		 * @param window Game window
		 */
		
		// Variables used for scaling elements
		float wFactor = static_cast<float>(window.width) / 1280;
		float hFactor = static_cast<float>(window.height) / 800;
		
		GameplayRects rects = {};

		rects.camTpRect           = {static_cast<int>(50 * wFactor), static_cast<int>(window.height - 160 * hFactor), static_cast<int>(200 * wFactor), static_cast<int>(50 * hFactor)};
		rects.playerTpRect        = {static_cast<int>(50 * wFactor), static_cast<int>(window.height - 100 * hFactor), static_cast<int>(200 * wFactor), static_cast<int>(50 * hFactor)};

		return rects;
	}
	GameRects InitGameRects(core::MF_Window &window) {
		/**
		 * @brief This function initilizes inventory rects
		 * @param window Game window
		 */
		
		// Variables used for scaling elements
		float wFactor = static_cast<float>(window.width) / 1280;
		float hFactor = static_cast<float>(window.height) / 800;
		
		GameRects rects = {};

		rects.gameInfoRect        = {static_cast<int>(window.width - 250 * wFactor), static_cast<int>(window.height - 160 * hFactor), static_cast<int>(200 * wFactor), static_cast<int>(50 * hFactor)};
		rects.exitRect            = {static_cast<int>(window.width - 250 * wFactor), static_cast<int>(window.height - 100 * hFactor), static_cast<int>(200 * wFactor), static_cast<int>(50 * hFactor)};

		return rects;
	}
    OtherRects InitOtherRects(core::MF_Window &window) {
		/**
		 * @brief This function initilizes inventory rects
		 * @param window Game window
		 */
		
		// Variables used for scaling elements
		float wFactor = static_cast<float>(window.width) / 1280;
		float hFactor = static_cast<float>(window.height) / 800;

		OtherRects rects = {};
		// Name                     X Offset                      Y Offset                 Width Height

		// TODO SET THEESE WHEN NEEDED
		/*rects.textureColorRect    = {window.width - 130,     50,                       80,  80 };
		rects.textureColorRectb   = {window.width - 135,     45,                       90,  90 };
		rects.textureColorTextRect= {window.width - 130,     140,                      80,  40 };*/

		


        rects.solidRect           = {50,                          window.height - 355, static_cast<int>(80 * wFactor), static_cast<int>(80 * hFactor)};
        rects.solidRectb          = {45,                          window.height - 360, static_cast<int>(90 * wFactor), static_cast<int>(90 * hFactor)};
        rects.solidTextRect       = {50,                          window.height - 260, static_cast<int>(80 * wFactor), static_cast<int>(40 * hFactor)};

		rects.presetRect          = {window.width / 2 - 85,  window.height - 160, static_cast<int>(170 * wFactor), static_cast<int>(115 * hFactor)};
		rects.presetTextRect      = {window.width / 2 - 40,  window.height - 160, static_cast<int>(80 * wFactor), static_cast<int>(115 * hFactor)};
		rects.presetTitleRect     = {window.width / 2 - 115, window.height - 215, static_cast<int>(230 * wFactor), static_cast<int>(50 * hFactor)};
		return rects;
	}
}