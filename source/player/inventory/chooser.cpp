#include "mf/core.h"
#include "mf/logic.h"

#include "cheats.h"
#include "colorpicker.h"
#include "files.h"
#include "inventory.h"
#include "presets.h"
#include "settings.h"
#include "textures.h"

namespace inventory {
    void Chooser(core::MF_Window &window, game::Settings &settings, game::Map &map, game::Player &player, game::Camera &camera, game::Preset (&preset)[10], inventory::rects &rects) {
		if (window.event.type == SDL_MOUSEBUTTONDOWN) {
			if (!settings.colorPicker && !settings.texturePicker) { // Checks if any color picker is active
				// Solid
				if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.solidRectb))
					settings.placeSolidBlocks = !settings.placeSolidBlocks;

				// Gameplay
				// Cam TP
				if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.camTpRect)) {
					settings.cheatsId = 1;
					settings.cheats = true;
					settings.inventory = false;
				}
				// Player TP
				else if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.playerTpRect)) {
					settings.cheatsId = 2;
					settings.cheats = true;
					settings.inventory = false;
				}

				// Game
				// Save
				if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.saveRect)) {
					files::SaveMap(map);
					files::SaveSettings(settings, player, camera);
				}
				// Load
				else if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.loadRect)) {
					files::LoadMap(map);
					files::LoadSettings(settings, player, camera, preset);
				}
				// Game Info
				else if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.gameInfoRect))
					settings.gameInfo = !settings.gameInfo;
				// Exit
				else if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.exitRect)) {
					settings.inventory = false;
					settings.dialogueId = 1;
					settings.dialogue = true;
				}
			}

			if(!settings.texturePicker) {
				// Color
				if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.colorRectb)) {
					settings.colorPicker = !settings.colorPicker;
					settings.colorPickerId = 1;
				}
				else if (settings.colorPicker && settings.colorPickerId == 1)
					preset[settings.curPreset].blockColor = colorpicker::Event(window, settings);

				// BG Color
				if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.bgColorRectb)) {
					settings.colorPicker = !settings.colorPicker;
					settings.colorPickerId = 2;
				}
				else if (settings.colorPicker && settings.colorPickerId == 2)
					settings.bgColor = colorpicker::Event(window, settings);

				// Player Color
				if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.playerColorRectb)) {
					settings.colorPicker = !settings.colorPicker;
					settings.colorPickerId = 3;
				}
				else if (settings.colorPicker && settings.colorPickerId == 3)
					settings.playerColor = colorpicker::Event(window, settings);
			}

			if(!settings.colorPicker) {
				// Texture
				if(logic::IsMouseTouching(window.mouseX, window.mouseY, rects.textureIdRectb))
					settings.texturePicker = !settings.texturePicker;
				else if (settings.texturePicker)
					settings.blockTextureId = textures::PickerEvent(window, settings);
			}
		}
	}
}