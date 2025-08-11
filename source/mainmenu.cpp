#include <iostream>

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"
#include "mf/logic.h"

#include "settings.h"

namespace game {
    void MainMenu(core::MF_Window &window, game::Game &game, bool &running) {
        bool runningmenu = true;
        //SDL_Event event = {};

        while(runningmenu) {
            SDL_GetMouseState(&window.mouse.x, &window.mouse.y);

            // Main rects
            SDL_Rect titleRect = {window.width / 2 - 300, window.height / 2 - 300, 600, 100};
            SDL_Rect newButtonRect = {window.width / 2 - 100, window.height / 2 - 10, 200, 75};
            SDL_Rect loadButtonRect = {window.width / 2 - 100, window.height / 2 + 100, 200, 75};

            // Bottom text rects
            SDL_Rect devRect = {0, window.height - 50, 250, 50};
            SDL_Rect publisherRect = {window.width - 500, window.height - 50, 500, 50};

            // Handle events
            while(SDL_PollEvent(&window.event) != 0) {
                switch(window.event.type) {
                    case SDL_QUIT:
                        running = false;
                        runningmenu = false;
                        break;
                    case SDL_WINDOWEVENT:
                        switch(window.event.window.event) {
                            case SDL_WINDOWEVENT_RESIZED:
                                // Handle resizing window
                                window.width = window.event.window.data1;
                                window.height = window.event.window.data2;
                                break;
                        }
                        break;

                    case SDL_KEYDOWN:
                        if(window.event.key.keysym.sym == SDLK_ESCAPE) {
                            running = false;
                            runningmenu = false;
                        }
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        if(window.event.button.button == SDL_BUTTON_LEFT) {
                            // Check if any button is clicked
                            if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, newButtonRect)) {
                                game.menuLoad = false; // Set menu load to false
                                runningmenu = false;
                            }
                            else if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, loadButtonRect)) {
                                game.menuLoad = true; // Set menu load to true
                                runningmenu = false;
                            }
                        }
                        break;

                    default:
                        break;
                }
            }

            // Clear screen
            draw::SetDrawColor(window.renderer, colors::colorID[0]); // Set background color
            SDL_RenderClear(window.renderer);

            // Draw stuff
            draw::DrawRect(window.renderer, titleRect, colors::colorID[1]);
            draw::DrawText(window.renderer, window.font, titleRect, "Random SDL Game", colors::colorID[31]); // Draw title

            draw::DrawButton(window.renderer, newButtonRect, colors::aqua, window.mouse.x, window.mouse.y); // Draw play button
            draw::DrawText(window.renderer, window.font, newButtonRect, "New game", colors::colorID[31]); // Draw button text
            draw::DrawButton(window.renderer, loadButtonRect, colors::aqua, window.mouse.x, window.mouse.y); // Draw play button
            draw::DrawText(window.renderer, window.font, loadButtonRect, "Load game", colors::colorID[31]); // Draw button text

            // Draw bottom texts
            draw::DrawText(window.renderer, window.font, devRect, "Dev: Mikicrep", colors::white);
            draw::DrawText(window.renderer, window.font, publisherRect, "Publisher: Mikicrep Studios", colors::white);

            // Present the renderer
            SDL_RenderPresent(window.renderer);
        }
    }
}
