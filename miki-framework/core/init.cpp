#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include "mf/core.h"

namespace core {
    bool InitWindow(core::MF_Window &window, std::string title, int width, int height) {
        /**
         * @brief Initilizes window and stuff related to it
         * @param window Stuct that contains information about window
         * @param title Title of the window
         * @param width Width of the window
         * @param height Height of the window
         */

        // Update settings
        window.width = width;
        window.height = height;

        // Init everything
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
            std::cerr << "[MF] SDL could not initialize! SDL Error: " << SDL_GetError() << '\n';
            return false;
        }

        if (TTF_Init() == -1) {
            std::cerr << "[MF] TTF could not initialize! SDL_ttf Error: " << TTF_GetError() << '\n';
            SDL_Quit();
            return false;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "[MF] SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "[MF] SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << '\n';
            IMG_Quit();
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        window.font = TTF_OpenFont("mf/font.ttf", 96);
        if (!window.font) {
            std::cerr << "[MF] Failed to load font. SDL_ttf Error: " << TTF_GetError() << '\n';
            Mix_CloseAudio();
            IMG_Quit();
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        // Create window and renderer
        window.window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window.width, window.height, SDL_WINDOW_RESIZABLE);
        if (!window.window) {
            std::cerr << "[MF] Window could not be created! SDL Error: " << SDL_GetError() << '\n';
            TTF_CloseFont(window.font);
            Mix_CloseAudio();
            IMG_Quit();
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        window.renderer = SDL_CreateRenderer(window.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!window.renderer) {
            std::cerr << "[MF] Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
            SDL_DestroyWindow(window.window);
            TTF_CloseFont(window.font);
            Mix_CloseAudio();
            IMG_Quit();
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND); // Transparency

        return true;
    }
}