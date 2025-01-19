#include <iostream>

#include "settings.h"

namespace debug {
    void PrintMap(game::Map map) {
        std::cout << "DEBUG - MAP" << std::endl;
        std::cout << "Width: " << map.width << std::endl
                  << "Height: " << map.height << std::endl;
    }
}