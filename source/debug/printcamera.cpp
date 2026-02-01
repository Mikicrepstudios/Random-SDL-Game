#include <iostream>

#include "settings.h"

namespace debug {
    void PrintCamera(game::Game &game) {
        /**
         * @brief Prints all values from game::Camera struct
         * @param cam Game camera
         */
        std::cout << "DEBUG - CAMERA" << std::endl;
        std::cout << "Scale: " << game.cam.scale << std::endl
                  << "OffsetX: " << game.cam.offSetX << std::endl
                  << "OffsetY: " << game.cam.offSetY << std::endl
                  << "Highlight: " << game.cam.highlight << std::endl;
    }
}