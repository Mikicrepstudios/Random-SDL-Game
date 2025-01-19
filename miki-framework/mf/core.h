#pragma once

#include <functional>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

/**
 * @brief Main struct for Mikicrep Framework colors
 */
struct MF_Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

/**
 * @brief Main struct for Mikicrep Framework pixels
 */
struct MF_Pixel {
    int x = 0;
    int y = 0;
};

/**
 * @brief This namespace contains stuff that are required for Mikicrep Framework to work correctly
 */
namespace core {
    struct MF_Window {
        bool fullscreen = false; // Is window fullscreened
        int width = 1280;
        int height = 800;
        int fps = 60; // THIS IS NOT FPS COUNTER, IT IS MAXIMUM FPS

        struct Mouse {
            bool isDown = false; // On when mouse button is held, off when its released
            int x = 0;
            int y = 0;
        } mouse;

        SDL_Window* window = {};
        SDL_Renderer* renderer = {};
        SDL_Event event = {};
        TTF_Font* font = {};

        // How long window existed
        int frame = 0;
        int timeS = 0;
        int timeM = 0;
        int timeH = 0;
    };

    // Big stuff
    bool InitWindow(core::MF_Window &window, std::string title, int width, int height);
    void Exit(MF_Window &window);
    void TimeCount(core::MF_Window &window);

    // Timers
    void AddTimer(Uint32 interval, std::function<void()> callback, bool repeat = true);
    void UpdateTimers();

    // Addional stuff
    void printver(int type);
    void SetWindowIcon(SDL_Window* window, std::string path);
    SDL_Texture* LoadImg(SDL_Renderer* renderer, std::string path);

    // 69 Chad function
    void miki(core::MF_Window window); // Best function ever
}