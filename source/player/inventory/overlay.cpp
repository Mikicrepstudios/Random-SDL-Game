#include <string>

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"

#include "colorpicker.h"
#include "inventory.h"
#include "settings.h"
#include "textures.h"

namespace inventory {
	void Overlay(core::MF_Window &window, game::Settings settings, inventory::rects &rects, textures::BlockTextures blockTextures[32]) {
		// Define variables
		int colorR, colorG, colorB = 0;

		if (settings.inventory) {
			// Render bg
			SDL_Rect bgRect = {25, 25, window.width - 50, window.height - 50};
			draw::DrawRect(window.renderer, bgRect, colors::darkgray);

			// Color
			draw::DrawRect(window.renderer, rects.colorRectb, colors::gray);
			draw::DrawRect(window.renderer, rects.colorRect, colors::colorID[settings.blockColor - 1]);
			draw::DrawText(window.renderer, window.font, rects.colorTextRect, "Block", colors::white);

			// BG Color
			draw::DrawRect(window.renderer, rects.bgColorRectb, colors::gray);
			draw::DrawRect(window.renderer, rects.bgColorRect, colors::colorID[settings.bgColor - 1]);
			draw::DrawText(window.renderer, window.font, rects.bgColorTextRect, "BG", colors::white);

			// Player Color
			draw::DrawRect(window.renderer, rects.playerColorRectb, colors::gray);
			draw::DrawRect(window.renderer, rects.playerColorRect, colors::colorID[settings.playerColor - 1]);
			draw::DrawText(window.renderer, window.font, rects.playerColorTextRect, "Player", colors::white);

			// Preview
			draw::DrawRect(window.renderer, rects.previewRectb, colors::gray);
			draw::DrawRect(window.renderer, rects.previewRect, colors::colorID[settings.blockColor - 1]);
			draw::DrawTextureRect(window.renderer, rects.previewRect, blockTextures[settings.blockTextureId].texture);
			draw::DrawText(window.renderer, window.font, rects.previewTextRect, "Preview", colors::white);

			// Textures
			draw::DrawRect(window.renderer, rects.textureIdRectb, colors::gray);
			draw::DrawTextureRect(window.renderer, rects.textureIdRect, blockTextures[settings.blockTextureId].texture);
			draw::DrawText(window.renderer, window.font, rects.textureIdTextRect, "Text. id", colors::white);

			// Solid
			draw::DrawRect(window.renderer, rects.solidRectb, colors::gray);
			if(settings.placeSolidBlocks) draw::DrawRect(window.renderer, rects.solidRect, colors::lightgreen);
			else draw::DrawRect(window.renderer, rects.solidRect, colors::red);
			draw::DrawText(window.renderer, window.font, rects.solidTextRect, "Solid", colors::white);
			
			// Presets
			draw::DrawRect(window.renderer, rects.presetRect, colors::gray);
			draw::DrawText(window.renderer, window.font, rects.presetTextRect, std::to_string(settings.curPreset + 1).c_str(), colors::white);
			draw::DrawText(window.renderer, window.font, rects.presetTitleRect, "Presets", colors::white);

			// Backgrounds
			draw::DrawRect(window.renderer, rects.bgGameplay, colors::gray);
			draw::DrawRect(window.renderer, rects.bgGame, colors::gray);

			// Titles
			draw::DrawText(window.renderer, window.font, rects.gameplayTextRect, "Gameplay", colors::white);
			draw::DrawText(window.renderer, window.font, rects.gameTextRect, "Game", colors::white);

			// Camtp button
			draw::DrawButton(window.renderer, rects.camTpRect, colors::aqua, window.mouseX, window.mouseY);
			draw::DrawText(window.renderer, window.font, rects.camTpRect, "Cam TP", colors::white);

			// Playertp button
			draw::DrawButton(window.renderer, rects.playerTpRect, colors::aqua, window.mouseX, window.mouseY);
			draw::DrawText(window.renderer, window.font, rects.playerTpRect, "Player TP", colors::white);

			// Save button
			draw::DrawButton(window.renderer, rects.saveRect, colors::lightgreen, window.mouseX, window.mouseY);
			draw::DrawText(window.renderer, window.font, rects.saveRect, "Save", colors::white);

			// Gameinfo button
			draw::DrawButton(window.renderer, rects.gameInfoRect, colors::darkgray, window.mouseX, window.mouseY);
			draw::DrawText(window.renderer, window.font, rects.gameInfoRect, "Game Info", colors::white);

			// Load button
			draw::DrawButton(window.renderer, rects.loadRect, colors::indigo, window.mouseX, window.mouseY);
			draw::DrawText(window.renderer, window.font, rects.loadRect, "Load", colors::white);

			// Exit button
			draw::DrawButton(window.renderer, rects.exitRect, colors::red, window.mouseX, window.mouseY);
			draw::DrawText(window.renderer, window.font, rects.exitRect, "Exit", colors::white);

			// Color pickers
			if(settings.colorPicker && settings.colorPickerId == 1)
				colorpicker::Overlay(window, settings);
			else if(settings.colorPicker && settings.colorPickerId == 2)
				colorpicker::Overlay(window, settings);
			else if(settings.colorPicker && settings.colorPickerId == 3)
				colorpicker::Overlay(window, settings);
			}

			if(settings.texturePicker)
				textures::PickerOverlay(window, blockTextures);
	}
}
