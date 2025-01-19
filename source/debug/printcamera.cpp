#include <iostream>

#include "settings.h"

namespace debug {
    void PrintCamera(game::Camera cam) {
        std::cout << "DEBUG - CAMERA" << std::endl;
        std::cout << "Scale: " << cam.scale << std::endl
                  << "OffsetX: " << cam.offSetX << std::endl
                  << "OffsetY: " << cam.offSetY << std::endl
                  << "Highlight: " << cam.highlight << std::endl;
    }
}