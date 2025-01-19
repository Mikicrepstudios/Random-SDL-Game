#include <iostream>

#include "settings.h"

namespace debug {
    void PrintMap(game::Map map) {
        std::cout << map.width << std::endl
                  << map.height << std::endl;
    }
}