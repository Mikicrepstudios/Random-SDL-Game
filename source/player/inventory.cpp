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
	rects initRects(int width, int height) {
		rects rects;
		rects.colorRect = {50, 50, 80, 80};
		rects.colorRectb = {45, 45, 90, 90};
		rects.colorTextRect = {50, 140, 80, 40};
		rects.bgColorRect = {width - 130, 50, 80, 80};
		rects.bgColorRectb = {width - 135, 45, 90, 90};
		rects.bgColorTextRect = {width - 130, 140, 80, 40};
		rects.playerColorRect = {50, 200, 80, 80};
		rects.playerColorRectb = {45, 195, 90, 90};
		rects.playerColorTextRect = {50, 290, 80, 40};
		rects.previewRect = {width / 2 - 50, 50, 100, 100};
		rects.previewRectb = {width / 2 - 55, 45, 110, 110};
		rects.previewTextRect = {width / 2 - 50, 160, 100, 40};
		rects.bgGameplay = {45, height - 215, 420, 170};
		rects.gameplayTextRect = {150, height - 210, 200, 50};
		rects.camTpRect = {50, height - 160, 200, 50};
		rects.playerTpRect = {50, height - 100, 200, 50};
		rects.bgGame = {width - 465, height - 215, 420, 170};
		rects.gameTextRect = {width - 360, height - 210, 200, 50};
		rects.saveRect = {width - 460, height - 160, 200, 50};
		rects.loadRect = {width - 460, height - 100, 200, 50};
		rects.gameInfoRect = {width - 250, height - 160, 200, 50};
		rects.exitRect = {width - 250, height - 100, 200, 50};
		rects.presetRect = {width / 2 - 85, height - 160, 170, 115};
		rects.presetTextRect = {width / 2 - 40, height - 160, 80, 115};
		rects.selCLPresetRect = {width / 2 - 115, height - 160, 20, 115};
		rects.selCRPresetRect = {width / 2 + 95, height - 160, 20, 115};
		rects.presetTitleRect = {width / 2 - 115, height - 215, 230, 50};
		return rects;
	}
}

namespace player {
	void InventoryEvent(SDL_Event event, bool &inventory, bool &colorPick, bool &bgColorPick, int &fps) {
		if(event.key.keysym.sym == SDLK_e) {
			if (!inventory) {
				inventory = true;
				fps = 15; // Reduce CPU usage
			}
			else {
				// Before stopping inv exit every sub UI
				colorPick = false;
				bgColorPick = false;

				inventory = false;
				fps = 60;
			}
		}
	}
	void MouseInvChooser(SDL_Renderer* renderer, SDL_Event event, inventory::rects rects, bool &inventory, bool &running, bool &highlight, bool &camTp, bool &playerTp, bool &colorPick, bool &bgColorPick, bool &playerColorPick, bool &gameInfo, Block worldMap[250][250], int mapWidth, int mapHeight, int &blockColor, int &bgColor, int &playerColor, int mouseX, int mouseY, int width, int height, int &playerX, int &playerY, int &camOffSetX, int &camOffSetY, int &camScale, game::Settings settings, game::Player player, game::Camera camera) {
		if (event.type == SDL_MOUSEBUTTONDOWN && inventory) {
			if (!colorPick && !bgColorPick) {
				// Gameplay
				if (mouseX >= rects.camTpRect.x && mouseX <= rects.camTpRect.x + rects.camTpRect.w &&
					mouseY >= rects.camTpRect.y && mouseY <= rects.camTpRect.y + rects.camTpRect.h) {
					camTp = !camTp;
					highlight = !highlight;
					inventory = false;
				}
				else if (mouseX >= rects.playerTpRect.x && mouseX <= rects.playerTpRect.x + rects.playerTpRect.w &&
					mouseY >= rects.playerTpRect.y && mouseY <= rects.playerTpRect.y + rects.playerTpRect.h) {
					playerTp = !playerTp;
					highlight = !highlight;
					inventory = false;
				}
				// Game
				if (mouseX >= rects.saveRect.x && mouseX <= rects.saveRect.x + rects.saveRect.w &&
				mouseY >= rects.saveRect.y && mouseY <= rects.saveRect.y + rects.saveRect.h) {
					files::SaveMap(worldMap, mapWidth, mapHeight);
					files::SaveSettings(settings, player, camera, blockColor);
				}
				else if (mouseX >= rects.loadRect.x && mouseX <= rects.loadRect.x + rects.loadRect.w &&
				mouseY >= rects.loadRect.y && mouseY <= rects.loadRect.y + rects.loadRect.h) {
					files::LoadMap(worldMap, mapWidth, mapHeight);
					files::LoadSettings(settings, player, camera, blockColor);
				}
				else if (mouseX >= rects.gameInfoRect.x && mouseX <= rects.gameInfoRect.x + rects.gameInfoRect.w &&
				mouseY >= rects.gameInfoRect.y && mouseY <= rects.gameInfoRect.y + rects.gameInfoRect.h && !colorPick && !bgColorPick)
					gameInfo = !gameInfo;
				else if (mouseX >= rects.exitRect.x && mouseX <= rects.exitRect.x + rects.exitRect.w &&
				mouseY >= rects.exitRect.y && mouseY <= rects.exitRect.y + rects.exitRect.h)
					running = false;
			}

			// Color
			if (mouseX >= rects.colorRectb.x && mouseX <= rects.colorRectb.x + rects.colorRectb.w &&
			mouseY >= rects.colorRectb.y && mouseY <= rects.colorRectb.y + rects.colorRectb.h && !bgColorPick && !playerColorPick)
				colorPick = !colorPick;
			else if (colorPick)
				player::ColorPickerEvent(colorPick, mouseX, mouseY, width, height, blockColor);

			// BG Color
			if (mouseX >= rects.bgColorRectb.x && mouseX <= rects.bgColorRectb.x + rects.bgColorRectb.w &&
			mouseY >= rects.bgColorRectb.y && mouseY <= rects.bgColorRectb.y + rects.bgColorRectb.h && !colorPick && !playerColorPick)
				bgColorPick = !bgColorPick;
			else if (bgColorPick)
				player::ColorPickerEvent(bgColorPick, mouseX, mouseY, width, height, bgColor);

			// Player Color
			if (mouseX >= rects.playerColorRectb.x && mouseX <= rects.playerColorRectb.x + rects.playerColorRectb.w &&
			mouseY >= rects.playerColorRectb.y && mouseY <= rects.playerColorRectb.y + rects.playerColorRectb.h && !colorPick && !bgColorPick)
				playerColorPick = !playerColorPick;
			else if (playerColorPick)
				player::ColorPickerEvent(playerColorPick, mouseX, mouseY, width, height, playerColor);
		}
	}
}

namespace overlay {
	void Inventory(SDL_Renderer* renderer, TTF_Font* font, inventory::rects rects, bool inventory, bool colorPick, bool bgcolorPick, bool playerColorPick, bool gameInfo, int blockColor, int bgColor, int playerColor, int width, int height, int mouseX, int mouseY, int preset) {
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
			if(colorPick)
				overlay::ColorPicker(renderer, font, "Block Color", width, height);
			else if(bgcolorPick)
				overlay::ColorPicker(renderer, font, "BG Color", width, height);
			else if(playerColorPick)
				overlay::ColorPicker(renderer, font, "Player Color", width, height);

			}
	}
}
