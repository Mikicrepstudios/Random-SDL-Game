#include "mf/core.h"

namespace core {
    void TimeCount(core::MF_Window &window) {
        /**
         * @brief This function increases time since window is spawned
         * @param sdlSettings Window that you want to cout time for
         */

        window.frame++;
        if(window.frame == window.fps) {
            window.frame = 0;
            window.timeS++;
            if(window.timeS == 60) {
                window.timeS = 0;
                window.timeM++;
                if(window.timeM == 60) {
                    window.timeM = 0;
                    window.timeH++;
                    // ...
                }
            }
        }
    }
}