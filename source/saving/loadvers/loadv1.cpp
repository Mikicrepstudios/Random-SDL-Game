#include <fstream>
#include <iostream>
#include <limits>

#include "settings.h"

namespace files {
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
}