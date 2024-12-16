#include <iostream>
#include <string>
#include <sstream>

#include "settings.h"

namespace commands {
    int TerminalMode(std::string args, int argsCount, bool &terminalmode) {
        if(argsCount == 0) {
            std::cout << "You need 1 argument [on/off]" << std::endl;
 
            return 4;
        }
        else if(argsCount == 1) {
            std::string* argsArray = new std::string[argsCount];
            std::stringstream ss(args);

            std::string curWord;
            int index = 0;
            while (ss >> curWord) {
                if (index < argsCount) {
                    argsArray[index++] = curWord;
                }
            }

            if(argsArray[0] == "on") {
                terminalmode = true;
                std::cout << "Terminal mode has been turned on successfully" << std::endl;
            }
            if(argsArray[0] == "off") {
                terminalmode = false;
                std::cout << "Terminal mode has been turned off successfully" << std::endl;
            }

            return 0;
        }
        else if(argsCount > 1) {
            return 3;
        }

        return -69;
    }
}