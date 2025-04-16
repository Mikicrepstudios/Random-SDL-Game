#include <iostream>
#include <string>
#include <sstream>

#include "game.h"
#include "settings.h"

namespace commands {
    int SetBlockTexture(std::string args, int argsCount, game::Settings &settings, game::Preset preset[10]) {
        /**
         * @brief Function for SetBlockTexture command
         * @param args Command arguments
         * @param argsCount Amount of command arguments
         * @param settings Game settings
         * @param preset Game presets
         */
        if(argsCount == 0) {
            return 2;
        }

        std::string* argsArray = new std::string[argsCount];
        std::stringstream ss(args);

        std::string curWord;
        int index = 0;
        while (ss >> curWord) {
            if (index < argsCount) {
                argsArray[index++] = curWord;
            }
        }

        int texture = std::stoi(argsArray[0]);

        // run
        if(argsCount == 1) {
            if(texture >= 0 && texture <= 15) {
                preset[settings.curPreset].textureId = texture;

                return 0;
            }
            else {
                std::cout << "Texture must be between 0 and 15" << std::endl;
                
                return 1;
            }
        }

        if(argsCount > 1) {
            return 3;
        }

        return -69;
    }
}