#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "files.h"
#include "game.h"
#include "settings.h"

namespace files {
    const int CURRENT_SAVE_VERSION = 1;

    void SaveGame(const game::Map& map, const game::Settings& settings, const game::Player& player, const game::Camera& cam, const std::string& slotName) {
        std::string folder = "saves/" + slotName;
        std::filesystem::create_directories(folder); // Make sure folder exists

        std::ofstream file(folder + "/savegame.msave");
        if (!file.is_open()) {
            std::cerr << "Failed to open save file for writing!\n";
            return;
        }

        file << "version " << CURRENT_SAVE_VERSION << "\n";

        // MAP section
        file << "[MAP]\n";
        file << map.width << " " << map.height << "\n";
        for (int x = 0; x < map.width; ++x) {
            for (int y = 0; y < map.height; ++y) {
                const auto& b = map.map[x][y];
                file << b.type << " " << b.color << " " << b.texture << " " << b.isSolid << "\n";
            }
        }

        // SETTINGS section
        file << "[SETTINGS]\n";
        file << settings.blockColor << " " << settings.bgColor << " ";
        file << player.x << " " << player.y << " " << settings.playerColor << " ";
        file << cam.offSetX << " " << cam.offSetY << " " << cam.scale << "\n";

        file.close();

        std::cout << "Game saved in slot: " << slotName << "\n";
    }

    // Main LoadGame function: detects version & dispatches to specific loaders
    void LoadGame(game::Map& map, game::Settings& settings, game::Player& player, game::Camera& cam, const std::string& saveName) {
        /*std::string folder = "saves/" + saveName;
        std::ifstream file(folder + "/savegame.msave");
        if (!file.is_open()) {
            std::cerr << "Save file not found for slot: " << saveName << "\n";
            return;
        }

        std::string line;
        if (!std::getline(file, line)) {
            std::cerr << "Save file empty or corrupt!\n";
            return;
        }

        int version = 0;
        if (line.rfind("version", 0) == 0) {
            version = std::stoi(line.substr(8));
        } else {
            std::cerr << "Save file missing version info!\n";
            return;
        }

        if (version == 1) {
            files::LoadGame_v1(map, settings, player, cam, file);
            std::cout << "Game loaded (v1) from slot: " << saveName << "\n";
        } else {
            std::cerr << "Unsupported save version: " << version << "\n";
            // Future versions here...
        }*/

        files::LoadLegacyMap(map);
    }

}
