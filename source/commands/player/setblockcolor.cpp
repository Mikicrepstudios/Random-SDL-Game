#include <iostream>
#include <string>
#include <sstream>

#include "presets.h"
#include "settings.h"

namespace commands {
    int SetBlockColor(std::string args, int argsCount, game::Settings &settings, game::Preset preset[10]) {
        if(argsCount == 0) {
            std::cout << "Not enough arguments" << std::endl;
            
            return 1;
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
            preset[settings.curPreset].blockColor = color;

            return 0;
        }

        if(argsCount > 1) {
            std::cout << "Too much arguments" << std::endl;
            return 1;
        }

        return -69;
    }
}