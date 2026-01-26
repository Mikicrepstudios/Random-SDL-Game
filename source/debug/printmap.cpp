#include <iostream>

#include "settings.h"

namespace debug {
    void PrintMap(game::Game &game) {
        /**
         * @brief Prints all values from game::Map struct
         * @param map Game map
         */
        std::cout << "DEBUG - MAP" << std::endl;
        std::cout << "Width: " << game.map.width << " blocks" << std::endl
                  << "Height: " << game.map.height << " blocks" << std::endl;
    }
}