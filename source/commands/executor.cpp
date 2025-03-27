#include <iostream>
#include <string>
#include <sstream>

#include "mf/core.h"

#include "commands.h"
#include "game.h"
#include "settings.h"

namespace commands {
    void Executor(std::string command, core::MF_Window &window, game::Game &game, game::Settings &settings, game::Map &map, game::Player &player, game::Camera &cam, game::Preset preset[10]) {
        /**
         * @brief Function that is used to execute game commands
         * @param command Whole command including arguments
         * @param window Game window
         * @param settings Game settings
         * @param map Game map
         * @param preset All player presets
         */
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
        else if(commandId == "getcampos") status = GetCamPos(cam);
        else if(commandId == "getplayercolor") status = GetPlayerColor(player);
        else if(commandId == "getplayerpos") status = GetPlayerPos(player);
        else if(commandId == "isblocksolid") status = IsBlockSolid(args, argsCount, map);
        else if(commandId == "terminalmode") status = TerminalMode(args, argsCount, game.terminalmode);

        // Player
        else if(commandId == "setbgcolor") status = SetBGColor(args, argsCount, settings);
        else if(commandId == "setblockcolor") status = SetBlockColor(args, argsCount, settings, preset);
        else if(commandId == "setblocktexture") status = SetBlockTexture(args, argsCount, settings, preset);
        else if(commandId == "setplayercolor") status = SetPlayerColor(args, argsCount, settings);
        else if(commandId == "tpcam") status = TpCam(args, argsCount, cam);
        else if(commandId == "tpplayer") status = TpPlayer(args, argsCount, map, player);

        // World
        else if(commandId == "destroy") status = Destroy(args, argsCount, map);
        else if(commandId == "place") status = Place(args, argsCount, settings, map);
        
        // No command found
        else std::cout << "Error executing command : Not found" << std::endl;

        // Command must return its response status, handle them here
        switch(status) {
            case 0:
                // Command executed successfully
                break;
            case 1:
                std::cout << "Error executing command" << std::endl;
                break;
            case 2:
                std::cout << "Too few arguments" << std::endl;
                break;
            case 3:
                std::cout << "Too many arguments" << std::endl;
                break;
            case 4:
                // Command gave custom response
                break;
            case -69:
                std::cout << "Something gone VERY wrong" << std::endl;
                break;
        }
    }
}