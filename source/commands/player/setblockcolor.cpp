#include <iostream>
#include <string>
#include <sstream>

#include "game.h"
#include "settings.h"

namespace commands {
    int SetBlockColor(std::string args, int argsCount, game::Settings &settings, game::Preset preset[10]) {
        /**
         * @brief Function for SetBlockColor command
         * @param args Command arguments
         * @param argsCount Amount of command arguments
         * @param settings Game settings
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

        int color = std::stoi(argsArray[0]);

        // run
        if(argsCount == 1) {
            if(color >= 1 && color <= 32) {
                preset[settings.curPreset].blockColor = color;

                return 0;
            }
            else {
                std::cout << "Color must be between 1 and 32" << std::endl;
                
                return 1;
            }
        }

        if(argsCount > 1) {
            return 3;
        }

        return -69;
    }
}