#include <string>

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"

#include "gui.h"
#include "inventory.h"
#include "settings.h"
#include "textures.h"

namespace inventory {
	void Overlay(core::MF_Window &window, game::Settings settings, inventory::MenuRects &menuRects, inventory::ColorRects &colorRects, inventory::DecalRects &decalRects, inventory::GameplayRects &gameplayRects, inventory::GameRects &gameRects, inventory::OtherRects &otherRects, textures::BlockTextures blockTextures[32]) {
		/**
		 * @brief This function draws inventory overlay
		 */

		if (settings.inventory) {
			// Render bg
			SDL_Rect bgRect = {25, 25, window.width - 50, window.height - 50};
			draw::DrawRect(window.renderer, bgRect, colors::darkgray);

			// Color
			draw::DrawRect(window.renderer, colorRects.colorRectb, colors::gray);
			draw::DrawButton(window.renderer, colorRects.colorRect, colors::colorID[settings.blockColor - 1], window.mouse.x, window.mouse.y);
			draw::DrawText(window.renderer, window.font, colorRects.colorTextRect, "Block", colors::white);

			// BG Color
			draw::DrawRect(window.renderer, colorRects.bgColorRectb, colors::gray);
			draw::DrawButton(window.renderer, colorRects.bgColorRect, colors::colorID[settings.bgColor - 1], window.mouse.x, window.mouse.y);
			draw::DrawText(window.renderer, window.font, colorRects.bgColorTextRect, "BG", colors::white);

			// Player Color
			draw::DrawRect(window.renderer, colorRects.playerColorRectb, colors::gray);
			draw::DrawButton(window.renderer, colorRects.playerColorRect, colors::colorID[settings.playerColor - 1], window.mouse.x, window.mouse.y);
			draw::DrawText(window.renderer, window.font, colorRects.playerColorTextRect, "Player", colors::white);

			// Preview
			draw::DrawRect(window.renderer, menuRects.previewRectb, colors::gray);
			draw::DrawRect(window.renderer, menuRects.previewRect, colors::colorID[settings.blockColor - 1]);
			draw::DrawTextureRect(window.renderer, menuRects.previewRect, blockTextures[settings.blockTextureId].texture);
			draw::DrawText(window.renderer, window.font, menuRects.previewTextRect, "Preview", colors::white);

			// Textures
			draw::DrawRect(window.renderer, decalRects.textureIdRectb, colors::gray);
			draw::DrawTextureRect(window.renderer, decalRects.textureIdRect, blockTextures[settings.blockTextureId].texture);
			draw::DrawText(window.renderer, window.font, decalRects.textureIdTextRect, "Text. id", colors::white);

			// Solid
			draw::DrawRect(window.renderer, otherRects.solidRectb, colors::gray);
			if(settings.placeSolidBlocks) draw::DrawButton(window.renderer, otherRects.solidRect, colors::lightgreen, window.mouse.x, window.mouse.y);
			else draw::DrawButton(window.renderer, otherRects.solidRect, colors::red, window.mouse.x, window.mouse.y);
			draw::DrawText(window.renderer, window.font, otherRects.solidTextRect, "Solid", colors::white);
			
			// Presets
			draw::DrawRect(window.renderer, otherRects.presetRect, colors::gray);
			draw::DrawText(window.renderer, window.font, otherRects.presetTextRect, std::to_string(settings.curPreset + 1).c_str(), colors::white);
			draw::DrawText(window.renderer, window.font, otherRects.presetTitleRect, "Presets", colors::white);

			// Backgrounds
			draw::DrawRect(window.renderer, menuRects.bgGameplay, colors::gray);
			draw::DrawRect(window.renderer, menuRects.bgGame, colors::gray);

			// Titles
			draw::DrawText(window.renderer, window.font, menuRects.gameplayTextRect, "Gameplay", colors::white);
			draw::DrawText(window.renderer, window.font, menuRects.gameTextRect, "Game", colors::white);

			// Camtp button
			draw::DrawButton(window.renderer, gameplayRects.camTpRect, colors::aqua, window.mouse.x, window.mouse.y);
			draw::DrawText(window.renderer, window.font, gameplayRects.camTpRect, "Cam TP", colors::white);

			// Playertp button
			draw::DrawButton(window.renderer, gameplayRects.playerTpRect, colors::aqua, window.mouse.x, window.mouse.y);
			draw::DrawText(window.renderer, window.font, gameplayRects.playerTpRect, "Player TP", colors::white);

			// Gameinfo button
			draw::DrawButton(window.renderer, gameRects.gameInfoRect, colors::darkgray, window.mouse.x, window.mouse.y);
			draw::DrawText(window.renderer, window.font, gameRects.gameInfoRect, "Game Info", colors::white);

			// Exit button
			draw::DrawButton(window.renderer, gameRects.exitRect, colors::red, window.mouse.x, window.mouse.y);
			draw::DrawText(window.renderer, window.font, gameRects.exitRect, "Exit", colors::white);

			// Color pickers
			if(settings.colorPicker)
				gui::ColorPickerOverlay(window, settings);

			if(settings.texturePicker)
				textures::PickerOverlay(window, blockTextures);
		}
	}
}
