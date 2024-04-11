#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "addional.h"
#include "block.h"
#include "files.h"
#include "graphics.h"
#include "overlay.h"
#include "player.h"

const int width = 1280;
const int height = 800;

// Color
SDL_Rect selCLRect = {50, 50, 20, 80};
SDL_Rect selCRRect = {170, 50, 20, 80};
SDL_Rect colorRect = {80, 50, 80, 80};
SDL_Rect colorTextRect = {80, 140, 80, 40};

// Background color
SDL_Rect selBCLRect = {width - 190, 50, 20, 80};
SDL_Rect selBCRRect = {width - 70, 50, 20, 80};
SDL_Rect bgColorRect = {width - 160, 50, 80, 80};
SDL_Rect bgColorTextRect = {width - 160, 140, 80, 40};

// Preview
SDL_Rect previewRect = {width / 2 - 50, 50, 100, 100};
SDL_Rect previewTextRect = {width / 2 - 50, 160, 100, 40};

// Buttons
SDL_Rect saveRect = {50, height - 160, 200, 50};
SDL_Rect loadRect = {50, height - 100, 200, 50};
SDL_Rect exitRect = {width - 250, height - 100, 200, 50};

namespace player {
    void InventoryEvent(SDL_Event event, bool &inventory, bool &colorPick, bool &bgColorPick) {
        if(event.key.keysym.sym == SDLK_e)
            // Before stopping inv exit every sub UI
            if (colorPick)
                colorPick = !colorPick;
            if (bgColorPick)
                bgColorPick = !bgColorPick;

            inventory = !inventory;
    }
    void MouseInvChooser(SDL_Renderer* renderer, SDL_Event event, bool inventory, bool &running, bool &colorPick, bool &bgColorPick, Block worldMap[250][250], int mapWidth, int mapHeight, int &curBlock, int &bgColor, int mouseX, int mouseY, int width, int height) {
        if (event.type == SDL_MOUSEBUTTONDOWN && inventory) {
            if (!colorPick && !bgColorPick) {
                // Bottom bar
                if (mouseX >= saveRect.x && mouseX <= saveRect.x + saveRect.w &&
                    mouseY >= saveRect.y && mouseY <= saveRect.y + saveRect.h)
                    files::SaveMap(event, worldMap, mapWidth, mapHeight);
                else if (mouseX >= loadRect.x && mouseX <= loadRect.x + loadRect.w &&
                    mouseY >= loadRect.y && mouseY <= loadRect.y + loadRect.h)
                    files::LoadMap(event, worldMap, mapWidth, mapHeight);
                else if (mouseX >= exitRect.x && mouseX <= exitRect.x + exitRect.w &&
                    mouseY >= exitRect.y && mouseY <= exitRect.y + exitRect.h)
                    running = false;
            }

            // Color
            if (mouseX >= colorRect.x && mouseX <= colorRect.x + colorRect.w &&
                mouseY >= colorRect.y && mouseY <= colorRect.y + colorRect.h && !bgColorPick)
                colorPick = !colorPick;

            // BG Color
            if (mouseX >= bgColorRect.x && mouseX <= bgColorRect.x + bgColorRect.w &&
                mouseY >= bgColorRect.y && mouseY <= bgColorRect.y + bgColorRect.h && !colorPick)
                bgColorPick = !bgColorPick;

            if (colorPick) {
                player::colorPickerEvent(mouseX, mouseY, width, height, curBlock);
            }
            else if (bgColorPick) {
                player::colorPickerEvent(mouseX, mouseY, width, height, bgColor);
            }
        }
    }
}

namespace overlay {
    void Inventory(SDL_Renderer* renderer, TTF_Font* font, bool inventory, bool colorPick, bool bgcolorPick, int curBlock, int &bgColor, int mouseX, int mouseY) {
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
            draw::DrawButton(renderer, selCLRect, 2, 1, mouseX, mouseY);
            draw::DrawButton(renderer, selCRRect, 2, 1, mouseX, mouseY);
            draw::DrawPreview(renderer, colorRect, curBlock);
            draw::DrawText(renderer, font, colorTextRect, "Block", textColor);

            // BG Color
            draw::DrawButton(renderer, selBCLRect, 2, 1, mouseX, mouseY);
            draw::DrawButton(renderer, selBCRRect, 2, 1, mouseX, mouseY);
            draw::DrawPreview(renderer, bgColorRect, bgColor);
            draw::DrawText(renderer, font, bgColorTextRect, "BG", textColor);

            // Preview
            draw::DrawPreview(renderer, previewRect, curBlock);
            draw::DrawText(renderer, font, previewTextRect, "Preview", textColor);

            // Save button
            draw::DrawButton(renderer, saveRect, 13, 12, mouseX, mouseY);
            draw::DrawText(renderer, font, saveRect, "save", textColor);

            // Load button
            draw::DrawButton(renderer, loadRect, 27, 26, mouseX, mouseY);
            draw::DrawText(renderer, font, loadRect, "load", textColor);

            // Exit button
            draw::DrawButton(renderer, exitRect, 27, 26, mouseX, mouseY);
            draw::DrawText(renderer, font, exitRect, "exit", textColor);

            // Color pickers
            if(colorPick | bgcolorPick)
                overlay::colorPicker(renderer, width, height);
        }
    }
}
