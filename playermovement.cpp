#include "SDL2/SDL.h"

namespace player {
    void PlayerMovement(SDL_Event event, int worldMap[250][250], int mapWidth, int mapHeight, int playerSpeed, int &playerX, int &playerY) {
        if(event.key.keysym.sym == SDLK_w && worldMap[playerX][playerY - playerSpeed] == 0 && playerY != 0) {
            worldMap[playerX][playerY] = 0;
            playerY -= playerSpeed;
        }
        else if(event.key.keysym.sym == SDLK_a && worldMap[playerX - playerSpeed][playerY] == 0 && playerX != 0) {
            worldMap[playerX][playerY] = 0;
            playerX -= playerSpeed;
        }
        else if(event.key.keysym.sym == SDLK_s && playerY != mapHeight && worldMap[playerX][playerY + playerSpeed] == 0) {
            worldMap[playerX][playerY] = 0;
            playerY += playerSpeed;
        }
        else if(event.key.keysym.sym == SDLK_d && playerX != mapWidth && worldMap[playerX + playerSpeed][playerY] == 0) {
            worldMap[playerX][playerY] = 0;
            playerX += playerSpeed;
        }
    }
}
