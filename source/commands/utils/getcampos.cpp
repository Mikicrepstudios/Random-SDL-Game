#include <iostream>

#include "settings.h"

namespace commands {
    int GetCamPos(game::Game &game) {
        /**
         * @brief Function for GetCamPos command
         * @param cam Game camera
         */
        
        std::cout << "Camera X: " << game.cam.offSetX << " Camera Y: " << game.cam.offSetY << std::endl;

        return 0;
    }
}