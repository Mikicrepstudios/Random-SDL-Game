#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <sstream>
#include <limits>

#include "game.h"
#include "settings.h"

namespace files {

const int CURRENT_SAVE_VERSION = 1;

// Version 1 load function
void LoadGame_v1(game::Map& map, game::Settings& settings, game::Player& player, game::Camera& cam, std::ifstream& file) {
    std::string line;
    while (std::getline(file, line)) {
        if (line == "[MAP]") {
            int w, h;
            if (!(file >> w >> h)) {
                std::cerr << "Failed to read map dimensions!\n";
                return;
            }
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            map.width = w;
            map.height = h;
            map.map.clear();
            map.map.resize(w);
            for (auto& col : map.map) col.resize(h);

            for (int x = 0; x < w; ++x) {
                for (int y = 0; y < h; ++y) {
                    int type, color, texture;
                    bool isSolid;
                    if (!(file >> type >> color >> texture >> isSolid)) {
                        std::cerr << "Failed reading block data at " << x << "," << y << "\n";
                        return;
                    }
                    map.map[x][y].type = type;
                    map.map[x][y].color = color;
                    map.map[x][y].texture = texture;
                    map.map[x][y].isSolid = isSolid;
                }
            }
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } 
        else if (line == "[SETTINGS]") {
            if (!(file >> settings.blockColor >> settings.bgColor)) {
                std::cerr << "Failed reading settings colors\n";
                return;
            }
            if (!(file >> player.x >> player.y >> settings.playerColor)) {
                std::cerr << "Failed reading player data\n";
                return;
            }
            if (!(file >> cam.offSetX >> cam.offSetY >> cam.scale)) {
                std::cerr << "Failed reading camera data\n";
                return;
            }
        } 
        else {
            // Ignore unknown sections or lines
        }
    }
}

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
    std::string folder = "saves/" + saveName;
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
        LoadGame_v1(map, settings, player, cam, file);
        std::cout << "Game loaded (v1) from slot: " << saveName << "\n";
    } else {
        std::cerr << "Unsupported save version: " << version << "\n";
        // Future versions here...
    }
}

}  // namespace files
