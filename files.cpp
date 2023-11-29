#include <fstream>
#include "SDL2/SDL.h"

namespace files {
    void savemapevent(SDL_Event event, int worldmap[250][250], int mapwidth, int mapheight) {
        // Open file
        std::ofstream mapfile("map.msave");

        // Save
        if (mapfile.is_open()) {
            for (int x = 0; x <= mapwidth; x++) {
                for (int y = 0; y <= mapheight; y++) {
                    if (worldmap[x][y] != 1) {
                        mapfile << worldmap[x][y] << " "; // Write blocks
                    }
                    else {
                        mapfile << 0 << " "; // Do not write if player
                    }
                }
                mapfile << "\n"; // Start a new line for the next row
            }
            mapfile.close();
        }
    }

    void loadmapevent(SDL_Event event, int worldmap[250][250], int mapwidth, int mapheight) {
        // Open file
        std::ifstream mapfile("map.msave");

        if (mapfile.is_open()) {
            for (int x = 0; x <= mapwidth; x++) {
                for (int y = 0; y <= mapheight; y++) {
                    mapfile >> worldmap[x][y];
                }
            }
            mapfile.close();
        }
    }
}
