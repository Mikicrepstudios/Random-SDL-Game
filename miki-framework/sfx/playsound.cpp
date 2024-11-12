#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_mixer.h"

namespace sfx {
    void PlaySound(const std::string& path) {
        /**
         * @brief This function plays sound, but pauses game while playing it
         * @param path Path to sound file you want to use
         */

        // Load sound
        Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
        if (!sound) {
            std::cerr << "Failed to load sound effect '" << path << "'. SDL_mixer Error: " << Mix_GetError() << '\n';
            return;
        }

        // Play play sound once
        if (Mix_PlayChannel(-1, sound, 0) == -1) {
            std::cerr << "Failed to play sound effect. SDL_mixer Error: " << Mix_GetError() << '\n';
            Mix_FreeChunk(sound);  // Free sound if it fails to play
            return;
        }

        // Wait until sound finishes
        while (Mix_Playing(-1) != 0) {
            SDL_Delay(100);  // Small delay to allow sound playback
        }

        // Free up memory
        Mix_FreeChunk(sound);
    }
}
