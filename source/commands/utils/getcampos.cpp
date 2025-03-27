#include <iostream>
#include <string>

#include "settings.h"

namespace commands {
    int GetCamPos(game::Camera &cam) {
        /**
         * @brief Function for GetCamPos command
         * @param cam Game camera
         */
        
        std::cout << "Camera X: " << -cam.offSetX << " Camera Y: " << -cam.offSetY << std::endl;

        return 0;
    }
}