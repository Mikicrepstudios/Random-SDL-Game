#pragma once

namespace files {
    extern void SaveMap(SDL_Event event, int worldMap[250][250], int mapWidth, int mapHeight);
    extern void LoadMap(SDL_Event event, int worldMap[250][250], int mapWidth, int mapHeight);
}
