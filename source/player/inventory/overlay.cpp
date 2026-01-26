#include <string>

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"

#include "gui.h"
#include "inventory.h"
#include "settings.h"
#include "textures.h"

namespace inventory {
	void DrawButtonINV(core::MF_Window &window, std::string text, MF_Color buttonColor, SDL_Rect buttonRect, SDL_Rect textRect) {
		// Function that draws button and text inside of button
		draw::DrawButton(window.renderer, buttonRect, buttonColor, window.mouse);
		draw::DrawText(window.renderer, window.font, textRect, text.c_str(), colors::white);
	}
	void DrawButtonWithBGINV(core::MF_Window &window, std::string text, MF_Color buttonColor, SDL_Rect bgRect, SDL_Rect buttonRect, SDL_Rect textRect) {
		// Function that draws background around button, button and text inside of button
		draw::DrawRect(window.renderer, bgRect, colors::gray);
		draw::DrawButton(window.renderer, buttonRect, buttonColor, window.mouse);
		draw::DrawText(window.renderer, window.font, textRect, text.c_str(), colors::white);
	}
	void DrawTextureButtonWithBGINV(core::MF_Window &window, std::string text, SDL_Texture* texture, SDL_Rect bgRect, SDL_Rect buttonRect, SDL_Rect textRect, int blockTextureId) {
		// Function that draws background around button, button with texture and text inside of button
		draw::DrawRect(window.renderer, bgRect, colors::gray);
		if(blockTextureId != 0) draw::DrawTextureRect(window.renderer, buttonRect, texture);
		draw::DrawText(window.renderer, window.font, textRect, text.c_str(), colors::white);
	}

	void Overlay(core::MF_Window &window, game::Game &game, inventory::MenuRects &menuRects, inventory::ColorRects &colorRects, inventory::DecalRects &decalRects, inventory::GameplayRects &gameplayRects, inventory::GameRects &gameRects, inventory::OtherRects &otherRects, textures::BlockTextures blockTextures[32]) {
		/**
		 * @brief This function draws inventory overlay
		 */
		// In some DrawButtonTextWithBG calls, textRect is modified to fit the text better

		auto& settings = game.settings;

		if (settings.inventory) {
			// Render bg
			SDL_Rect bgRect = {25, 25, window.width - 50, window.height - 50};
			draw::DrawRect(window.renderer, bgRect, colors::darkgray);

			// Left side
			DrawButtonWithBGINV(window, "Block", colors::colorID[settings.blockColor - 1], colorRects.colorRectb, colorRects.colorRect, colorRects.colorTextRect);
			DrawButtonWithBGINV(window, "BG", colors::colorID[settings.bgColor - 1], colorRects.bgColorRectb, colorRects.bgColorRect, {colorRects.bgColorTextRect.x + 10, colorRects.bgColorTextRect.y, colorRects.bgColorTextRect.w - 20, colorRects.bgColorTextRect.h});
			DrawButtonWithBGINV(window, "Player", colors::colorID[settings.playerColor - 1], colorRects.playerColorRectb, colorRects.playerColorRect, colorRects.playerColorTextRect);

			// Preview
			draw::DrawRect(window.renderer, menuRects.previewRectb, colors::gray);
			draw::DrawRect(window.renderer, menuRects.previewRect, colors::colorID[settings.blockColor - 1]);
			if(settings.blockTextureId != 0) draw::DrawTextureRect(window.renderer, menuRects.previewRect, blockTextures[settings.blockTextureId].texture);
			draw::DrawText(window.renderer, window.font, menuRects.previewTextRect, "Preview", colors::white);

			// Right side
			DrawTextureButtonWithBGINV(window, "Text. id", blockTextures[settings.blockTextureId].texture, decalRects.textureIdRectb, decalRects.textureIdRect, decalRects.textureIdTextRect, settings.blockTextureId);

			// Solid
			draw::DrawRect(window.renderer, otherRects.solidRectb, colors::gray);
			if(settings.placeSolidBlocks) draw::DrawButton(window.renderer, otherRects.solidRect, colors::lightgreen, window.mouse);
			else draw::DrawButton(window.renderer, otherRects.solidRect, colors::red, window.mouse);
			draw::DrawText(window.renderer, window.font, otherRects.solidTextRect, "Solid", colors::white);
			
			// Presets
			draw::DrawRect(window.renderer, otherRects.presetRect, colors::gray);
			draw::DrawText(window.renderer, window.font, otherRects.presetTextRect, std::to_string(settings.curPreset + 1).c_str(), colors::white);
			draw::DrawText(window.renderer, window.font, otherRects.presetTitleRect, "Presets", colors::white);

			// Bottom left stuff
			draw::DrawRect(window.renderer, menuRects.bgGameplay, colors::gray); // Background
			draw::DrawText(window.renderer, window.font, menuRects.gameplayTextRect, "Gameplay", colors::white); // Title

			DrawButtonINV(window, "Cam TP", colors::aqua, gameplayRects.camTpRect, gameplayRects.camTpRect);
			DrawButtonINV(window, "Player TP", colors::aqua, gameplayRects.playerTpRect, gameplayRects.playerTpRect);

			// Bottom right stuff
			draw::DrawRect(window.renderer, menuRects.bgGame, colors::gray); // Background
			draw::DrawText(window.renderer, window.font, {menuRects.gameTextRect.x + 25, menuRects.gameTextRect.y, menuRects.gameTextRect.w - 50, menuRects.gameTextRect.h}, "Game", colors::white); // Title
			DrawButtonINV(window, "Game Info", colors::darkgray, gameRects.gameInfoRect, gameRects.gameInfoRect);
			DrawButtonINV(window, "Exit", colors::red, gameRects.exitRect, {gameRects.exitRect.x + 40, gameRects.exitRect.y, gameRects.exitRect.w - 80, gameRects.exitRect.h});


			// Color pickers
			if(settings.colorPicker)
				gui::ColorPickerOverlay(window, game);

			if(settings.texturePicker)
				textures::PickerOverlay(window, blockTextures);
		}
	}
}
