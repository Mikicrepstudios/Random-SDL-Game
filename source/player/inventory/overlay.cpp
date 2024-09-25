#include <string>

#include "colorpicker.h"
#include "draw.h"
#include "inventory.h"
#include "settings.h"

namespace inventory {
	void Overlay(game::SDL_Settings sdlSettings, game::Settings settings, inventory::rects rects) {
		// Define variables
		SDL_Renderer* renderer = sdlSettings.renderer;
		TTF_Font* font = sdlSettings.font;
		int mouseX = sdlSettings.mouseX;
		int mouseY = sdlSettings.mouseY;

		int colorR, colorG, colorB = 0;
		SDL_Color textColor = {255, 255, 255};

		if (settings.inventory) {
			// Render bg
			SDL_Rect bgRect = {25, 25, sdlSettings.width - 50, sdlSettings.height - 50};
			draw::DrawRect(renderer, bgRect, 3);

			// Color
			draw::DrawRect(renderer, rects.colorRectb, 2);
			draw::DrawRect(renderer, rects.colorRect, settings.blockColor);
			draw::DrawText(renderer, font, rects.colorTextRect, "Block", textColor);

			// BG Color
			draw::DrawRect(renderer, rects.bgColorRectb, 2);
			draw::DrawRect(renderer, rects.bgColorRect, settings.bgColor);
			draw::DrawText(renderer, font, rects.bgColorTextRect, "BG", textColor);

			// Player Color
			draw::DrawRect(renderer, rects.playerColorRectb, 2);
			draw::DrawRect(renderer, rects.playerColorRect, settings.playerColor);
			draw::DrawText(renderer, font, rects.playerColorTextRect, "Player", textColor);

			// Preview
			draw::DrawRect(renderer, rects.previewRectb, 2);
			draw::DrawRect(renderer, rects.previewRect, settings.blockColor);
			draw::DrawText(renderer, font, rects.previewTextRect, "Preview", textColor);

			// Solid
			draw::DrawRect(renderer, rects.solidRectb, 2);
			if(settings.placeSolidBlocks) draw::DrawRect(renderer, rects.solidRect, 13);
			else draw::DrawRect(renderer, rects.solidRect, 27);
			draw::DrawText(renderer, font, rects.solidTextRect, "Solid", textColor);
			
			// Presets
			draw::DrawRect(renderer, rects.presetRect, 2);
			draw::DrawText(renderer, font, rects.presetTextRect, std::to_string(settings.curPreset + 1).c_str(), textColor);
			draw::DrawText(renderer, font, rects.presetTitleRect, "Presets", textColor);

			// Backgrounds
			draw::DrawRect(renderer, rects.bgGameplay, 2);
			draw::DrawRect(renderer, rects.bgGame, 2);

			// Titles
			draw::DrawText(renderer, font, rects.gameplayTextRect, "Gameplay", textColor);
			draw::DrawText(renderer, font, rects.gameTextRect, "Game", textColor);

			// Camtp button
			draw::DrawButton(renderer, rects.camTpRect, 7, mouseX, mouseY);
			draw::DrawText(renderer, font, rects.camTpRect, "Cam TP", textColor);

			// Playertp button
			draw::DrawButton(renderer, rects.playerTpRect, 7, mouseX, mouseY);
			draw::DrawText(renderer, font, rects.playerTpRect, "Player TP", textColor);

			// Save button
			draw::DrawButton(renderer, rects.saveRect, 13, mouseX, mouseY);
			draw::DrawText(renderer, font, rects.saveRect, "Save", textColor);

			// Gameinfo button
			draw::DrawButton(renderer, rects.gameInfoRect, 3, mouseX, mouseY);
			draw::DrawText(renderer, font, rects.gameInfoRect, "Game Info", textColor);

			// Load button
			draw::DrawButton(renderer, rects.loadRect, 6, mouseX, mouseY);
			draw::DrawText(renderer, font, rects.loadRect, "Load", textColor);

			// Exit button
			draw::DrawButton(renderer, rects.exitRect, 27, mouseX, mouseY);
			draw::DrawText(renderer, font, rects.exitRect, "Exit", textColor);

			// Color pickers
			if(settings.colorPicker && settings.colorPickerId == 1)
				colorpicker::Overlay(sdlSettings, settings);
			else if(settings.colorPicker && settings.colorPickerId == 2)
				colorpicker::Overlay(sdlSettings, settings);
			else if(settings.colorPicker && settings.colorPickerId == 3)
				colorpicker::Overlay(sdlSettings, settings);

			}
	}
}
