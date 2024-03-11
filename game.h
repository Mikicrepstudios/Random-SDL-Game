#pragma once

namespace events {
    extern void Camera(SDL_Event event, bool inventory, int &camOffSetX, int &camOffSetY, int &camScale);
}

namespace game {
    extern void RenderMap(SDL_Renderer* renderer, int worldMap[250][250], int mapWidth, int mapHeight, int camOffSetX, int camOffSetY, int camScale);
}

namespace gamemap {
    extern void ClearMap(int worldMap[250][250], int mapWidth, int mapHeight);
}
