#include <iostream>

#include "mf/core.h"
#include "mf/extra.h"

#include "inventory.h"

namespace inventory {
	MenuRects InitMenuRects(core::MF_Window &window) {
		/**
		 * @brief This function initilizes inventory rects
		 * @param window Game window
		 */

		// Variables used for scaling elements
		float wFactor = extra::castToFloat(window.width) / 1280;
		float hFactor = extra::castToFloat(window.height) / 800;
		
		MenuRects rects = {};

		rects.previewRect         = {window.width / 2 - 50, extra::castToInt(50 * hFactor), extra::castToInt(100 * wFactor), extra::castToInt(100 * hFactor)};
		rects.previewRectb        = {window.width / 2 - 55, extra::castToInt(45 * hFactor), extra::castToInt(110 * wFactor), extra::castToInt(110 * hFactor)};
		rects.previewTextRect     = {window.width / 2 - 50, extra::castToInt(160 * hFactor), extra::castToInt(100 * wFactor), extra::castToInt(40 * hFactor)};

		rects.bgGameplay          = {extra::castToInt(45 * wFactor), extra::castToInt(window.height - 215 * hFactor), extra::castToInt(420 * wFactor), extra::castToInt(170 * hFactor)};
		rects.gameplayTextRect    = {extra::castToInt(150 * wFactor), extra::castToInt(window.height - 210 * hFactor), extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor) };
		rects.bgGame              = {extra::castToInt(window.width - 465 * wFactor), extra::castToInt(window.height - 215 * hFactor), extra::castToInt(420 * wFactor), extra::castToInt(170 * hFactor)};
		rects.gameTextRect        = {extra::castToInt(window.width - 360 * wFactor), extra::castToInt(window.height - 210 * hFactor), extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};

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

		rects.colorRect           = {extra::castToInt(50 * wFactor), extra::castToInt(50 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(80 * hFactor)};
		rects.colorRectb          = {extra::castToInt(45 * wFactor), extra::castToInt(45 * hFactor), extra::castToInt(90 * wFactor), extra::castToInt(90 * hFactor)};
		rects.colorTextRect       = {extra::castToInt(50 * wFactor), extra::castToInt(140 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(40 * hFactor)};

		rects.bgColorRect         = {extra::castToInt(155 * wFactor), extra::castToInt(50 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(80 * hFactor)};
		rects.bgColorRectb        = {extra::castToInt(150 * wFactor), extra::castToInt(45 * hFactor), extra::castToInt(90 * wFactor), extra::castToInt(90 * hFactor)};
		rects.bgColorTextRect     = {extra::castToInt(155 * wFactor), extra::castToInt(140 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(40 * hFactor)};

		rects.playerColorRect     = {extra::castToInt(50 * wFactor), extra::castToInt(200 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(80 * hFactor)};
		rects.playerColorRectb    = {extra::castToInt(45 * wFactor), extra::castToInt(195 * hFactor), extra::castToInt(90 * wFactor), extra::castToInt(90 * hFactor)};
		rects.playerColorTextRect = {extra::castToInt(50 * wFactor), extra::castToInt(290 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(40 * hFactor)};

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

		rects.textureIdRect       = {extra::castToInt(window.width - 130 * wFactor), extra::castToInt(50 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(80 * hFactor)};
		rects.textureIdRectb      = {extra::castToInt(window.width - 135 * wFactor), extra::castToInt(45 * hFactor), extra::castToInt(90 * wFactor), extra::castToInt(90 * hFactor)};
		rects.textureIdTextRect   = {extra::castToInt(window.width - 130 * wFactor), extra::castToInt(140 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(40 * hFactor)};

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

		rects.camTpRect           = {extra::castToInt(50 * wFactor), extra::castToInt(window.height - 160 * hFactor), extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};
		rects.playerTpRect        = {extra::castToInt(50 * wFactor), extra::castToInt(window.height - 100 * hFactor), extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};

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

		rects.gameInfoRect        = {extra::castToInt(window.width - 250 * wFactor), extra::castToInt(window.height - 160 * hFactor), extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};
		rects.exitRect            = {extra::castToInt(window.width - 250 * wFactor), extra::castToInt(window.height - 100 * hFactor), extra::castToInt(200 * wFactor), extra::castToInt(50 * hFactor)};

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

        rects.solidRect           = {extra::castToInt(50 * wFactor), extra::castToInt(window.height - 355 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(80 * hFactor)};
        rects.solidRectb          = {extra::castToInt(45 * wFactor), extra::castToInt(window.height - 360 * hFactor), extra::castToInt(90 * wFactor), extra::castToInt(90 * hFactor)};
        rects.solidTextRect       = {extra::castToInt(50 * wFactor), extra::castToInt(window.height - 260 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(40 * hFactor)};

		rects.presetRect          = {extra::castToInt(window.width / 2 - 85 * wFactor), extra::castToInt(window.height - 160 * hFactor), extra::castToInt(170 * wFactor), extra::castToInt(115 * hFactor)};
		rects.presetTextRect      = {extra::castToInt(window.width / 2 - 40 * wFactor), extra::castToInt(window.height - 160 * hFactor), extra::castToInt(80 * wFactor), extra::castToInt(115 * hFactor)};
		rects.presetTitleRect     = {extra::castToInt(window.width / 2 - 115 * wFactor), extra::castToInt(window.height - 215 * hFactor), extra::castToInt(230 * wFactor), extra::castToInt(50 * hFactor)};
		return rects;
	}
}