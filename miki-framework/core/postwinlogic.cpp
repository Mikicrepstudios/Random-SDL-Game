#include "mf/core.h"

namespace core {
    void PostWindowLogic(core::MF_Window &window) {
        // FPS counter
        window.fpsAdv.frameCount++;
        Uint32 currentTime = SDL_GetTicks(); // Or SDL_GetPerformanceCounter()
        if (currentTime - window.fpsAdv.lastTime >= 1000) // Check if one second has passed
        {
            window.fpsAdv.realFPS = (float)window.fpsAdv.frameCount / ((currentTime - window.fpsAdv.lastTime) / 1000.0f);
            // Render 'fps' value to screen using SDL_ttf
            // ...
            window.fpsAdv.frameCount = 0;
            window.fpsAdv.lastTime = currentTime;
        }


        core::UpdateTimers();
        core::TimeCount(window);

        // Show frame
        SDL_RenderPresent(window.renderer);
        SDL_Delay(1000 / window.fps);
    }
}