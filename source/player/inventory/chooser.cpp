#include "cheats.h"
#include "colorpicker.h"
#include "files.h"
#include "inventory.h"
#include "presets.h"
#include "settings.h"

namespace inventory {
    void Chooser(game::SDL_Settings &sdlSettings, game::Settings &settings, game::Map &map, game::Player &player, game::Camera &camera, game::Preset preset[10], inventory::rects rects) {
		if (sdlSettings.event.type == SDL_MOUSEBUTTONDOWN && settings.inventory) {
			int mouseX = sdlSettings.mouseX;
			int mouseY = sdlSettings.mouseY;

			int width = sdlSettings.width;
			int height = sdlSettings.height;

			if (!settings.colorPicker) { // Checks if any color picker is active
				// Solid
				if (mouseX >= rects.solidRectb.x && mouseX <= rects.solidRectb.x + rects.solidRectb.w &&
					mouseY >= rects.solidRectb.y && mouseY <= rects.solidRectb.y + rects.solidRectb.h) {
					settings.placeSolidBlocks = !settings.placeSolidBlocks;
				}

				// Gameplay
				// Cam TP
				if (mouseX >= rects.camTpRect.x && mouseX <= rects.camTpRect.x + rects.camTpRect.w &&
					mouseY >= rects.camTpRect.y && mouseY <= rects.camTpRect.y + rects.camTpRect.h) {
					settings.cheatsId = 1;
					settings.cheats = true;
					settings.inventory = false;
				}
				// Player TP
				else if (mouseX >= rects.playerTpRect.x && mouseX <= rects.playerTpRect.x + rects.playerTpRect.w &&
					mouseY >= rects.playerTpRect.y && mouseY <= rects.playerTpRect.y + rects.playerTpRect.h) {
					settings.cheatsId = 2;
					settings.cheats = true;
					settings.inventory = false;
				}
				// Game
				// Save
				if (mouseX >= rects.saveRect.x && mouseX <= rects.saveRect.x + rects.saveRect.w &&
				mouseY >= rects.saveRect.y && mouseY <= rects.saveRect.y + rects.saveRect.h) {
					files::SaveMap(map);
					files::SaveSettings(settings, player, camera);
				}
				// Load
				else if (mouseX >= rects.loadRect.x && mouseX <= rects.loadRect.x + rects.loadRect.w &&
				mouseY >= rects.loadRect.y && mouseY <= rects.loadRect.y + rects.loadRect.h) {
					files::LoadMap(map);
					files::LoadSettings(settings, player, camera);
				}
				// Game Info
				else if (mouseX >= rects.gameInfoRect.x && mouseX <= rects.gameInfoRect.x + rects.gameInfoRect.w &&
				mouseY >= rects.gameInfoRect.y && mouseY <= rects.gameInfoRect.y + rects.gameInfoRect.h)
					settings.gameInfo = !settings.gameInfo;
				// Exit
				else if (mouseX >= rects.exitRect.x && mouseX <= rects.exitRect.x + rects.exitRect.w &&
				mouseY >= rects.exitRect.y && mouseY <= rects.exitRect.y + rects.exitRect.h) {
					settings.inventory = false;
					settings.dialogueId = 1;
					settings.dialogue = true;
				}
			}

			// Color
			if (mouseX >= rects.colorRectb.x && mouseX <= rects.colorRectb.x + rects.colorRectb.w &&
			mouseY >= rects.colorRectb.y && mouseY <= rects.colorRectb.y + rects.colorRectb.h && !settings.colorPicker) {
				settings.colorPicker = !settings.colorPicker;
				settings.colorPickerId = 1;
			}
			else if (settings.colorPicker && settings.colorPickerId == 1)
				preset[settings.curPreset].blockColor = colorpicker::Event(sdlSettings, settings);

			// BG Color
			if (mouseX >= rects.bgColorRectb.x && mouseX <= rects.bgColorRectb.x + rects.bgColorRectb.w &&
			mouseY >= rects.bgColorRectb.y && mouseY <= rects.bgColorRectb.y + rects.bgColorRectb.h && !settings.colorPicker) {
				settings.colorPicker = !settings.colorPicker;
				settings.colorPickerId = 2;
			}
			else if (settings.colorPicker && settings.colorPickerId == 2)
				settings.bgColor = colorpicker::Event(sdlSettings, settings);

			// Player Color
			if (mouseX >= rects.playerColorRectb.x && mouseX <= rects.playerColorRectb.x + rects.playerColorRectb.w &&
			mouseY >= rects.playerColorRectb.y && mouseY <= rects.playerColorRectb.y + rects.playerColorRectb.h && !settings.colorPicker) {
				settings.colorPicker = !settings.colorPicker;
				settings.colorPickerId = 3;
			}
			else if (settings.colorPicker && settings.colorPickerId == 3)
				settings.playerColor = colorpicker::Event(sdlSettings, settings);
		}
	}
}