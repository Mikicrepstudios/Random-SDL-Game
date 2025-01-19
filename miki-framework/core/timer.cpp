#include <SDL.h>
#include <vector>
#include <functional>

/*
// Example usage:
core::AddTimer(5000, [](){std::cout << "Execute every 5s" << std::endl;}, true);
core::AddTimer(500, [](){std::cout << "Execute once after 500ms" << std::endl;}, false);
*/

namespace core {
    struct Timer {
        Uint32 lastExecution;
        Uint32 interval;
        std::function<void()> callback;
        bool repeat; // Determines if the timer should repeat
    };

    std::vector<Timer> timers;

    void AddTimer(Uint32 interval, std::function<void()> callback, bool repeat = true) {
        /**
         * @brief This adds function that will be executed after specified amount of time, check usage in timer.cpp
         * @param interval Run timer every x miliseconds
         * @param callback Function that will be ran (can also just be basic block of code [](){code})
         * @param repeat Should timer be executed only once or every x miliseconds
         */

        timers.push_back({ SDL_GetTicks(), interval, callback, repeat });
    }

    void UpdateTimers() {
        /**
         * @brief Run added timers if needed
         */

        Uint32 currentTime = SDL_GetTicks();

        // Use an iterator to allow removing timers during iteration
        for (auto it = timers.begin(); it != timers.end();) {
            if (currentTime - it->lastExecution >= it->interval) {
                it->callback();
                if (it->repeat) {
                    it->lastExecution = currentTime;
                    ++it;
                } else {
                    // Remove non-repeating timers
                    it = timers.erase(it);
                }
            } else {
                ++it;
            }
        }
    }
}