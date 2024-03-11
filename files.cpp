#include <fstream>
#include "SDL2/SDL.h"

namespace files {
    void SaveMap(SDL_Event event, int worldMap[250][250], int mapWidth, int mapHeight) {
        // Open file
        std::ofstream mapFile("map.msave");

        // Save
        if (mapFile.is_open()) {
            for (int x = 0; x <= mapWidth; x++) {
                for (int y = 0; y <= mapHeight; y++) {
                    if (worldMap[x][y] != 1) {
                        mapFile << worldMap[x][y] << " "; // Write blocks
                    }
                    else {
                        mapFile << 0 << " "; // Do not write if player
                    }
                }
                mapFile << "\n"; // Start a new line for the next row
            }
            mapFile.close();
        }
    }

    void LoadMap(SDL_Event event, int worldMap[250][250], int mapWidth, int mapHeight) {
        // Open file
        std::ifstream mapFile("map.msave");

        if (mapFile.is_open()) {
            for (int x = 0; x <= mapWidth; x++) {
                for (int y = 0; y <= mapHeight; y++) {
                    mapFile >> worldMap[x][y];
                }
            }
            mapFile.close();
        }
    }
}
