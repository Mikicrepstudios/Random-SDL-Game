#include <iostream>

#include "settings.h"

namespace debug {
    void PrintCamera(game::Camera cam) {
        std::cout << cam.scale << std::endl
                  << cam.offSetX << std::endl
                  << cam.offSetY << std::endl
                  << cam.highlight << std::endl;
    }
}