#include <iostream>
#include <string>
#include "SDL_mixer.h"

namespace sfx {
    void PlayBGMusic(const std::string path) {
        /**
         * @brief This function plays music in background
         * @param path Path to music file
         */

        // First we load music file
        Mix_Music* music = Mix_LoadMUS(path.c_str());
        if (!music) {
            std::cerr << "[MF] Failed to load music file '" << path << "'. SDL_mixer Error: " << Mix_GetError() << '\n';
            return;
        }

        // Then we play it in background
        if (Mix_PlayMusic(music, -1) == -1) {
            std::cerr << "[MF] Failed to play music. SDL_mixer Error: " << Mix_GetError() << '\n';
            Mix_FreeMusic(music);
            return;
        }
    }
}