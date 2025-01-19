#include <iostream>

#include "settings.h"

namespace debug {
    void PrintCamera(game::Camera cam) {
        /**
         * @brief Prints all values from game::Camera struct
         * @param cam Game camera
         */
        std::cout << "DEBUG - CAMERA" << std::endl;
        std::cout << "Scale: " << cam.scale << std::endl
                  << "OffsetX: " << cam.offSetX << std::endl
                  << "OffsetY: " << cam.offSetY << std::endl
                  << "Highlight: " << cam.highlight << std::endl;
    }
}