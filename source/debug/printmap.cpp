#include <iostream>

#include "settings.h"

namespace debug {
    void PrintMap(game::Map map) {
        /**
         * @brief Prints all values from game::Map struct
         * @param map Game map
         */
        std::cout << "DEBUG - MAP" << std::endl;
        std::cout << "Width: " << map.width << std::endl
                  << "Height: " << map.height << std::endl;
    }
}