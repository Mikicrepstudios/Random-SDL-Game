#include <iostream>

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"
#include "mf/logic.h"

namespace game {
    void MainMenu(core::MF_Window &window, bool &running) {
        bool runningmenu = true;
        //SDL_Event event = {};

        while(runningmenu) {
            SDL_GetMouseState(&window.mouse.x, &window.mouse.y);

            SDL_Rect titleRect = {window.width / 2 - 300, window.height / 2 - 300, 600, 100};
            SDL_Rect playButtonRect = {window.width / 2 - 100, window.height / 2 + 20, 200, 100};

            // Handle events
            while(SDL_PollEvent(&window.event) != 0) {
                switch(window.event.type) {
                    case SDL_QUIT:
                        running = false;
                        runningmenu = false;
                        break;

                    case SDL_KEYDOWN:
                        if(window.event.key.keysym.sym == SDLK_ESCAPE) {
                            running = false;
                            runningmenu = false;
                        }
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        if(window.event.button.button == SDL_BUTTON_LEFT) {
                            // Check if play button is clicked
                            if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, playButtonRect))
                                runningmenu = false;
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

            draw::DrawButton(window.renderer, playButtonRect, colors::colorID[12], window.mouse.x, window.mouse.y); // Draw play button
            draw::DrawText(window.renderer, window.font, playButtonRect, "Play", colors::colorID[31]); // Draw button text

            // Present the renderer
            SDL_RenderPresent(window.renderer);
        }
    }
}
