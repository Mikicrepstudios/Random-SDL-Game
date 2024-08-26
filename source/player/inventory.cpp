#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <string>

#include "addional.h"
#include "block.h"
#include "files.h"
#include "inventory.h"
#include "overlay.h"
#include "player.h"
#include "presets.h"
#include "settings.h"

namespace inventory {
	rects InitRects(game::SDL_Settings sdlSettings) {
		// widthFactor and heightFactor // TODO //
		/*float wFactor = sdlSettings.width / 1280;
		float hFactor = sdlSettings.height / 800;*/

		rects rects = {};
		// Name                     X Offset                      Y Offset                 Width Height
		rects.colorRect           = {50,                          50,                       80,  80 };
		rects.colorRectb          = {45,                          45,                       90,  90 };
		rects.colorTextRect       = {50,                          140,                      80,  40 };
		rects.bgColorRect         = {sdlSettings.width - 130,     50,                       80,  80 };
		rects.bgColorRectb        = {sdlSettings.width - 135,     45,                       90,  90 };
		rects.bgColorTextRect     = {sdlSettings.width - 130,     140,                      80,  40 };
		rects.playerColorRect     = {50,                          200,                      80,  80 };
		rects.playerColorRectb    = {45,                          195,                      90,  90 };
		rects.playerColorTextRect = {50,                          290,                      80,  40 };
		rects.previewRect         = {sdlSettings.width / 2 - 50,  50,                       100, 100};
		rects.previewRectb        = {sdlSettings.width / 2 - 55,  45,                       110, 110};
		rects.previewTextRect     = {sdlSettings.width / 2 - 50,  160,                      100, 40 };
		rects.bgGameplay          = {45,                          sdlSettings.height - 215, 420, 170};
		rects.gameplayTextRect    = {150,                         sdlSettings.height - 210, 200, 50 };
		rects.camTpRect           = {50,                          sdlSettings.height - 160, 200, 50 };
		rects.playerTpRect        = {50,                          sdlSettings.height - 100, 200, 50 };
		rects.bgGame              = {sdlSettings.width - 465,     sdlSettings.height - 215, 420, 170};
		rects.gameTextRect        = {sdlSettings.width - 360,     sdlSettings.height - 210, 200, 50 };
		rects.saveRect            = {sdlSettings.width - 460,     sdlSettings.height - 160, 200, 50 };
		rects.loadRect            = {sdlSettings.width - 460,     sdlSettings.height - 100, 200, 50 };
		rects.gameInfoRect        = {sdlSettings.width - 250,     sdlSettings.height - 160, 200, 50 };
		rects.exitRect            = {sdlSettings.width - 250,     sdlSettings.height - 100, 200, 50 };
		rects.presetRect          = {sdlSettings.width / 2 - 85,  sdlSettings.height - 160, 170, 115};
		rects.presetTextRect      = {sdlSettings.width / 2 - 40,  sdlSettings.height - 160, 80,  115};
		rects.selCLPresetRect     = {sdlSettings.width / 2 - 115, sdlSettings.height - 160, 20,  115};
		rects.selCRPresetRect     = {sdlSettings.width / 2 + 95,  sdlSettings.height - 160, 20,  115};
		rects.presetTitleRect     = {sdlSettings.width / 2 - 115, sdlSettings.height - 215, 230, 50 };
		return rects;
	}
}

namespace player {
	void InventoryEvent(SDL_Event event, game::Settings &settings) {
		if(event.key.keysym.sym == SDLK_e) {
			if (!settings.inventory) {
				settings.inventory = true;
			}
			else {
				// Before stopping inv exit every sub UI
				settings.colorPicker = false;
				settings.inventory = false;
			}
		}
	}
	void MouseInvChooser(SDL_Renderer* renderer, SDL_Event event, inventory::rects rects, game::SDL_Settings &sdlSettings, game::Settings &settings, game::Map &map, game::Player &player, game::Camera &camera, game::Cheats &cheats, game::Preset preset[10]) {
		if (event.type == SDL_MOUSEBUTTONDOWN && settings.inventory) {
			int mouseX = sdlSettings.mouseX;
			int mouseY = sdlSettings.mouseY;

			int width = sdlSettings.width;
			int height = sdlSettings.height;

			if (!settings.colorPicker) { // Checks if any color picker is active
				// Gameplay
				// Cam TP
				if (mouseX >= rects.camTpRect.x && mouseX <= rects.camTpRect.x + rects.camTpRect.w &&
					mouseY >= rects.camTpRect.y && mouseY <= rects.camTpRect.y + rects.camTpRect.h) {
					cheats.camTp = !cheats.camTp;
					camera.highlight = !camera.highlight;
					settings.inventory = false;
				}
				// Player TP
				else if (mouseX >= rects.playerTpRect.x && mouseX <= rects.playerTpRect.x + rects.playerTpRect.w &&
					mouseY >= rects.playerTpRect.y && mouseY <= rects.playerTpRect.y + rects.playerTpRect.h) {
					cheats.playerTp = !cheats.playerTp;
					camera.highlight = !camera.highlight;
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
				preset[settings.curPreset].blockColor = player::ColorPickerEvent(sdlSettings, settings);

			// BG Color
			if (mouseX >= rects.bgColorRectb.x && mouseX <= rects.bgColorRectb.x + rects.bgColorRectb.w &&
			mouseY >= rects.bgColorRectb.y && mouseY <= rects.bgColorRectb.y + rects.bgColorRectb.h && !settings.colorPicker) {
				settings.colorPicker = !settings.colorPicker;
				settings.colorPickerId = 2;
			}
			else if (settings.colorPicker && settings.colorPickerId == 2)
				settings.bgColor = player::ColorPickerEvent(sdlSettings, settings);

			// Player Color
			if (mouseX >= rects.playerColorRectb.x && mouseX <= rects.playerColorRectb.x + rects.playerColorRectb.w &&
			mouseY >= rects.playerColorRectb.y && mouseY <= rects.playerColorRectb.y + rects.playerColorRectb.h && !settings.colorPicker) {
				settings.colorPicker = !settings.colorPicker;
				settings.colorPickerId = 3;
			}
			else if (settings.colorPicker && settings.colorPickerId == 3)
				settings.playerColor = player::ColorPickerEvent(sdlSettings, settings);
		}
	}
}

namespace overlay {
	void Inventory(SDL_Renderer* renderer, TTF_Font* font, inventory::rects rects, bool inventory, bool gameInfo, int blockColor, int bgColor, int playerColor, int width, int height, int mouseX, int mouseY, int preset, game::SDL_Settings sdlSettings, game::Settings settings) {
		// Define variables
		int colorR, colorG, colorB = 0;
		SDL_Color textColor = {255, 255, 255};

		if (inventory) {
			// Render bg
			SDL_Rect bgRect = {25, 25, width - 50, height - 50};
			draw::DrawRect(renderer, bgRect, 3);

			// Color
			draw::DrawRect(renderer, rects.colorRectb, 2);
			draw::DrawRect(renderer, rects.colorRect, blockColor);
			draw::DrawText(renderer, font, rects.colorTextRect, "Block", textColor);

			// BG Color
			draw::DrawRect(renderer, rects.bgColorRectb, 2);
			draw::DrawRect(renderer, rects.bgColorRect, bgColor);
			draw::DrawText(renderer, font, rects.bgColorTextRect, "BG", textColor);

			// Player Color
			draw::DrawRect(renderer, rects.playerColorRectb, 2);
			draw::DrawRect(renderer, rects.playerColorRect, playerColor);
			draw::DrawText(renderer, font, rects.playerColorTextRect, "Player", textColor);

			// Preview
			draw::DrawRect(renderer, rects.previewRectb, 2);
			draw::DrawRect(renderer, rects.previewRect, blockColor);
			draw::DrawText(renderer, font, rects.previewTextRect, "Preview", textColor);
			
			// Presets
			draw::DrawRect(renderer, rects.presetRect, 2);
			draw::DrawText(renderer, font, rects.presetTextRect, std::to_string(preset + 1).c_str(), textColor);
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
				overlay::ColorPicker(sdlSettings, settings);
			else if(settings.colorPicker && settings.colorPickerId == 2)
				overlay::ColorPicker(sdlSettings, settings);
			else if(settings.colorPicker && settings.colorPickerId == 3)
				overlay::ColorPicker(sdlSettings, settings);

			}
	}
}
