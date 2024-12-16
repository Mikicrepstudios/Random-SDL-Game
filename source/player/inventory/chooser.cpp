#include <iostream>

#include "mf/core.h"
#include "mf/logic.h"

#include "cheats.h"
#include "files.h"
#include "game.h"
#include "gui.h"
#include "inventory.h"
#include "settings.h"
#include "textures.h"

namespace inventory {
    void Chooser(core::MF_Window &window, game::Settings &settings, game::Player &player, game::Camera &camera, game::Preset preset[10], inventory::MenuRects &menuRects, inventory::ColorRects &colorRects, inventory::DecalRects &decalRects, inventory::GameplayRects &gameplayRects, inventory::GameRects &gameRects, inventory::OtherRects &otherRects) {
		/**
		 * @brief This function on mouse click checks which option on inventory is clicked
		 */
		
		if (window.event.type == SDL_MOUSEBUTTONDOWN) {
			if (!settings.colorPicker && !settings.texturePicker) { // Checks if any color picker is active
				// Solid
				if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, otherRects.solidRectb))
					settings.placeSolidBlocks = !settings.placeSolidBlocks;

				// Gameplay
				// Cam TP
				if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, gameplayRects.camTpRect)) {
					settings.cheatsId = 1;
					settings.cheats = true;
					settings.inventory = false;
				}
				// Player TP
				else if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, gameplayRects.playerTpRect)) {
					settings.cheatsId = 2;
					settings.cheats = true;
					settings.inventory = false;
				}

				// Game
				// Game Info
				else if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, gameRects.gameInfoRect))
					settings.gameInfo = !settings.gameInfo;
				// Exit
				else if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, gameRects.exitRect)) {
					settings.inventory = false;
					settings.dialogueId = 1;
					settings.dialogue = true;
				}
			}

			if(!settings.texturePicker) {
				// Color
				if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, colorRects.colorRectb)) {
					settings.colorPicker = !settings.colorPicker;
					settings.colorPickerId = 1;
				}
				else if (settings.colorPicker && settings.colorPickerId == 1)
					preset[settings.curPreset].blockColor = gui::ColorPickerEvent(window, settings);

				// BG Color
				if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, colorRects.bgColorRectb)) {
					settings.colorPicker = !settings.colorPicker;
					settings.colorPickerId = 2;
				}
				else if (settings.colorPicker && settings.colorPickerId == 2)
					settings.bgColor = gui::ColorPickerEvent(window, settings);

				// Player Color
				if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, colorRects.playerColorRectb)) {
					settings.colorPicker = !settings.colorPicker;
					settings.colorPickerId = 3;
				}
				else if (settings.colorPicker && settings.colorPickerId == 3)
					settings.playerColor = gui::ColorPickerEvent(window, settings);
			}

			if(!settings.colorPicker) {
				// Texture
				if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, decalRects.textureIdRectb))
					settings.texturePicker = !settings.texturePicker;
				else if (settings.texturePicker)
					settings.blockTextureId = textures::PickerEvent(window, settings);
			}
		}
	}
}