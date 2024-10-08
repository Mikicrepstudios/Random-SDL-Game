#include <iostream>
#include <string>
#include <sstream>

#include "commands.h"
#include "presets.h"
#include "settings.h"

namespace commands {
    void Executor(std::string command, game::SDL_Settings sdlSettings, game::Settings settings, game::Map &map, game::Preset preset[10]) {
        int status = 0;
        size_t pos = command.find(' ');

        std::string commandId = command.substr(0, pos);
        std::string args = command.substr(pos + 1);
        int argsCount = 0;

        // Get amount of args
       if (commandId == args) {
            argsCount = 0; // If no arguments then set to 0
        }
        else {
            std::istringstream stream(args);
            std::string curWord = "";

            while (stream >> curWord) {
                ++argsCount;
            }
        }

        // Utils
        if(commandId == "echo") status = Echo(args);
        else if(commandId == "getbgcolor") status = GetBGColor(settings.bgColor);
        else if(commandId == "getblocktype") status = GetBlockType(args, argsCount, map);
        else if(commandId == "getblockcolor") status = GetBlockColor(args, argsCount, map);
        else if(commandId == "isblocksolid") status = IsBlockSolid(args, argsCount, map);

        // Player
        else if(commandId == "setblockcolor") status = SetBlockColor(args, argsCount, settings, preset);

        // World
        else if(commandId == "destroy") status = Destroy(args, argsCount, map);
        else if(commandId == "place") status = Place(args, argsCount, settings, map);
        
        // Handle errors
        // If no command was found
        else std::cout << "Error executing command : Not found" << std::endl;

        // If command for any reason fails to run successfully write this
        if(status == 1) std::cout << "Error executing command" << std::endl;
        else if(status == -69) std::cout << "Something gone VERY wrong" << std::endl;
    }
}