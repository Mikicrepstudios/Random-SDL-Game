#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <string>

#include "addional.h"
#include "block.h"
#include "files.h"
#include "graphics.h"
#include "overlay.h"
#include "player.h"
#include "preset.h"

const int width = 1280;
const int height = 800;

// Color
SDL_Rect colorRect = {50, 50, 80, 80};
SDL_Rect colorRectb = {45, 45, 90, 90};
SDL_Rect colorTextRect = {50, 140, 80, 40};

// Background color
SDL_Rect bgColorRect = {width - 130, 50, 80, 80};
SDL_Rect bgColorRectb = {width - 135, 45, 90, 90};
SDL_Rect bgColorTextRect = {width - 130, 140, 80, 40};

// Player color
SDL_Rect playerColorRect = {50, 200, 80, 80};
SDL_Rect playerColorRectb = {45, 195, 90, 90};
SDL_Rect playerColorTextRect = {50, 290, 80, 40};

// Preview
SDL_Rect previewRect = {width / 2 - 50, 50, 100, 100};
SDL_Rect previewRectb = {width / 2 - 55, 45, 110, 110};
SDL_Rect previewTextRect = {width / 2 - 50, 160, 100, 40};

// Gameplay related buttons
SDL_Rect bgGameplay = {45, height - 215, 420, 170};
SDL_Rect gameplayTextRect = {150, height - 210, 200, 50};
SDL_Rect camTpRect = {50, height - 160, 200, 50};
SDL_Rect playerTpRect = {50, height - 100, 200, 50};

// Game related buttons
SDL_Rect bgGame = {width - 465, height - 215, 420, 170};
SDL_Rect gameTextRect = {width - 360, height - 210, 200, 50};
SDL_Rect saveRect = {width - 460, height - 160, 200, 50};
SDL_Rect loadRect = {width - 460, height - 100, 200, 50};
SDL_Rect gameInfoRect = {width - 250, height - 160, 200, 50};
SDL_Rect exitRect = {width - 250, height - 100, 200, 50};

// Presets related buttons
SDL_Rect presetRect = {width / 2 - 85, height - 160, 170, 115};
SDL_Rect presetTextRect = {width / 2 - 40, height - 160, 80, 115};
SDL_Rect selCLPresetRect = {width / 2 - 115, height - 160, 20, 115};
SDL_Rect selCRPresetRect = {width / 2 + 95, height - 160, 20, 115};
SDL_Rect presetTitleRect = {width / 2 - 115, height - 215, 230, 50};

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
	void MouseInvChooser(SDL_Renderer* renderer, SDL_Event event, bool &inventory, bool &running, bool &highlight, bool &camTp, bool &playerTp, bool &colorPick, bool &bgColorPick, bool &playerColorPick, bool &gameInfo, Block worldMap[250][250], int mapWidth, int mapHeight, int &blockColor, int &bgColor, int &playerColor, int mouseX, int mouseY, int width, int height, int &playerX, int &playerY, int &camOffSetX, int &camOffSetY, int &camScale) {
		if (event.type == SDL_MOUSEBUTTONDOWN && inventory) {
			if (!colorPick && !bgColorPick) {
				// Gameplay
				if (mouseX >= camTpRect.x && mouseX <= camTpRect.x + camTpRect.w &&
					mouseY >= camTpRect.y && mouseY <= camTpRect.y + camTpRect.h) {
					camTp = !camTp;
					highlight = !highlight;
					inventory = false;
				}
				else if (mouseX >= playerTpRect.x && mouseX <= playerTpRect.x + playerTpRect.w &&
					mouseY >= playerTpRect.y && mouseY <= playerTpRect.y + playerTpRect.h) {
					playerTp = !playerTp;
					highlight = !highlight;
					inventory = false;
				}
				// Game
				if (mouseX >= saveRect.x && mouseX <= saveRect.x + saveRect.w &&
				mouseY >= saveRect.y && mouseY <= saveRect.y + saveRect.h) {
					files::SaveMap(worldMap, mapWidth, mapHeight);
					files::SaveSettings(playerX, playerY, camOffSetX, camOffSetY, camScale, playerColor, blockColor, bgColor);
				}
				else if (mouseX >= loadRect.x && mouseX <= loadRect.x + loadRect.w &&
				mouseY >= loadRect.y && mouseY <= loadRect.y + loadRect.h) {
					files::LoadMap(worldMap, mapWidth, mapHeight);
					files::LoadSettings(playerX, playerY, camOffSetX, camOffSetY, camScale, playerColor, blockColor, bgColor);
				}
				else if (mouseX >= gameInfoRect.x && mouseX <= gameInfoRect.x + gameInfoRect.w &&
				mouseY >= gameInfoRect.y && mouseY <= gameInfoRect.y + gameInfoRect.h && !colorPick && !bgColorPick)
					gameInfo = !gameInfo;
				else if (mouseX >= exitRect.x && mouseX <= exitRect.x + exitRect.w &&
				mouseY >= exitRect.y && mouseY <= exitRect.y + exitRect.h)
					running = false;
			}

			// Color
			if (mouseX >= colorRectb.x && mouseX <= colorRectb.x + colorRectb.w &&
			mouseY >= colorRectb.y && mouseY <= colorRectb.y + colorRectb.h && !bgColorPick && !playerColorPick)
				colorPick = !colorPick;
			else if (colorPick)
				player::ColorPickerEvent(colorPick, mouseX, mouseY, width, height, blockColor);

			// BG Color
			if (mouseX >= bgColorRectb.x && mouseX <= bgColorRectb.x + bgColorRectb.w &&
			mouseY >= bgColorRectb.y && mouseY <= bgColorRectb.y + bgColorRectb.h && !colorPick && !playerColorPick)
				bgColorPick = !bgColorPick;
			else if (bgColorPick)
				player::ColorPickerEvent(bgColorPick, mouseX, mouseY, width, height, bgColor);

			// Player Color
			if (mouseX >= playerColorRectb.x && mouseX <= playerColorRectb.x + playerColorRectb.w &&
			mouseY >= playerColorRectb.y && mouseY <= playerColorRectb.y + playerColorRectb.h && !colorPick && !bgColorPick)
				playerColorPick = !playerColorPick;
			else if (playerColorPick)
				player::ColorPickerEvent(playerColorPick, mouseX, mouseY, width, height, playerColor);
		}
	}
}

namespace overlay {
	void Inventory(SDL_Renderer* renderer, TTF_Font* font, bool inventory, bool colorPick, bool bgcolorPick, bool playerColorPick, bool gameInfo, int blockColor, int bgColor, int playerColor, int mouseX, int mouseY, int preset) {
		// Define variables
		int colorR, colorG, colorB = 0;
		SDL_Color textColor = {255, 255, 255};

		if (inventory) {
			// Render bg
			graphics::GetColor(3, colorR, colorG, colorB);
			SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
			SDL_Rect bgRect = {25, 25, width - 50, height - 50};
			SDL_RenderFillRect(renderer, &bgRect);

			// Color
			draw::DrawPreview(renderer, colorRectb, 2);
			draw::DrawPreview(renderer, colorRect, blockColor);
			draw::DrawText(renderer, font, colorTextRect, "Block", textColor);

			// BG Color
			draw::DrawPreview(renderer, bgColorRectb, 2);
			draw::DrawPreview(renderer, bgColorRect, bgColor);
			draw::DrawText(renderer, font, bgColorTextRect, "BG", textColor);

			// Player Color
			draw::DrawPreview(renderer, playerColorRectb, 2);
			draw::DrawPreview(renderer, playerColorRect, playerColor);
			draw::DrawText(renderer, font, playerColorTextRect, "Player", textColor);

			// Preview
			draw::DrawPreview(renderer, previewRectb, 2);
			draw::DrawPreview(renderer, previewRect, blockColor);
			draw::DrawText(renderer, font, previewTextRect, "Preview", textColor);
			
			// Presets
			draw::DrawPreview(renderer, presetRect, 2);
			draw::DrawButton(renderer, selCLPresetRect, 2, mouseX, mouseY);
			draw::DrawButton(renderer, selCRPresetRect, 2, mouseX, mouseY);
			draw::DrawText(renderer, font, presetTextRect, std::to_string(preset).c_str(), textColor);
			draw::DrawText(renderer, font, presetTitleRect, "Presets", textColor);

			// Backgrounds
			draw::DrawPreview(renderer, bgGameplay, 2);
			draw::DrawPreview(renderer, bgGame, 2);

			// Titles
			draw::DrawText(renderer, font, gameplayTextRect, "Gameplay", textColor);
			draw::DrawText(renderer, font, gameTextRect, "Game", textColor);

			// Camtp button
			draw::DrawButton(renderer, camTpRect, 7, mouseX, mouseY);
			draw::DrawText(renderer, font, camTpRect, "Cam TP", textColor);

			// Playertp button
			draw::DrawButton(renderer, playerTpRect, 7, mouseX, mouseY);
			draw::DrawText(renderer, font, playerTpRect, "Player TP", textColor);

			// Save button
			draw::DrawButton(renderer, saveRect, 13, mouseX, mouseY);
			draw::DrawText(renderer, font, saveRect, "Save", textColor);

			// Gameinfo button
			draw::DrawButton(renderer, gameInfoRect, 3, mouseX, mouseY);
			draw::DrawText(renderer, font, gameInfoRect, "Game Info", textColor);

			// Load button
			draw::DrawButton(renderer, loadRect, 6, mouseX, mouseY);
			draw::DrawText(renderer, font, loadRect, "Load", textColor);

			// Exit button
			draw::DrawButton(renderer, exitRect, 27, mouseX, mouseY);
			draw::DrawText(renderer, font, exitRect, "Exit", textColor);

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
