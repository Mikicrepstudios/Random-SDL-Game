#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "files.h"
#include "graphics.h"
#include "addional.h"

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
    void InventoryEvent(SDL_Event event, bool &inventory) {
        if(event.key.keysym.sym == SDLK_e)
            inventory = !inventory;
    }
    void MouseInvChooser(SDL_Event event, bool inventory, bool &running, int worldMap[250][250], int mapWidth, int mapHeight, int &curBlock, int &bgColor, int mouseX, int mouseY) {
        if (event.type == SDL_MOUSEBUTTONDOWN && inventory) {
            // Color
            if (mouseX >= selCLRect.x && mouseX <= selCLRect.x + selCLRect.w &&
                mouseY >= selCLRect.y && mouseY <= selCLRect.y + selCLRect.h) {
                if (curBlock == 10)
                    curBlock = 25;
                else
                    curBlock -= 1;
            }
            else if (mouseX >= selCRRect.x && mouseX <= selCRRect.x + selCRRect.w &&
                     mouseY >= selCRRect.y && mouseY <= selCRRect.y + selCRRect.h) {
                if (curBlock == 25)
                    curBlock = 10;
                else
                    curBlock += 1;
            }

            // BG Color
            if (mouseX >= selBCLRect.x && mouseX <= selBCLRect.x + selBCLRect.w &&
                mouseY >= selBCLRect.y && mouseY <= selBCLRect.y + selBCLRect.h) {
                if (bgColor == 0)
                    bgColor = 15;
                else
                    bgColor -= 1;
            }
            else if (mouseX >= selBCRRect.x && mouseX <= selBCRRect.x + selBCRRect.w &&
                     mouseY >= selBCRRect.y && mouseY <= selBCRRect.y + selBCRRect.h) {
                if (bgColor == 15)
                    bgColor = 0;
                else
                    bgColor += 1;
            }

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
    }
}

namespace overlay {
    void Inventory(SDL_Renderer* renderer, TTF_Font* font, bool inventory, int curBlock, int bgColor, int mouseX, int mouseY) {
        // Define variables
        int colorR, colorG, colorB = 0;
        SDL_Color textColor = {255, 255, 255};

        if (inventory) {
            // Render bg
            graphics::GetColor(8, colorR, colorG, colorB);
            SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
            SDL_Rect bgRect = {25, 25, width - 50, height - 50};
            SDL_RenderFillRect(renderer, &bgRect);

            // Color
            draw::DrawButton(renderer, selCLRect, 15, 7, mouseX, mouseY);
            draw::DrawButton(renderer, selCRRect, 15, 7, mouseX, mouseY);
            draw::DrawPreview(renderer, colorRect, curBlock - 10);
            draw::DrawText(renderer, font, colorTextRect, "Block", textColor);

            // BG Color
            draw::DrawButton(renderer, selBCLRect, 15, 7, mouseX, mouseY);
            draw::DrawButton(renderer, selBCRRect, 15, 7, mouseX, mouseY);
            draw::DrawPreview(renderer, bgColorRect, bgColor);
            draw::DrawText(renderer, font, bgColorTextRect, "BG", textColor);

            // Preview
            draw::DrawPreview(renderer, previewRect, curBlock - 10);
            draw::DrawText(renderer, font, previewTextRect, "Preview", textColor);

            // Save button
            draw::DrawButton(renderer, saveRect, 2, 3, mouseX, mouseY);
            draw::DrawText(renderer, font, saveRect, "save", textColor);

            // Load button
            draw::DrawButton(renderer, loadRect, 9, 1, mouseX, mouseY);
            draw::DrawText(renderer, font, loadRect, "load", textColor);

            // Exit button
            draw::DrawButton(renderer, exitRect, 9, 1, mouseX, mouseY);
            draw::DrawText(renderer, font, exitRect, "exit", textColor);
        }
    }
}
