#include "SDL2/SDL.h"

#include "block.h"
#include "settings.h"

namespace player {
	void PlayerMovement(SDL_Event event, Block worldMap[250][250], int mapWidth, int mapHeight, game::Player &player) {
		if(event.key.keysym.sym == SDLK_w && worldMap[player.x][player.y - player.speed].type == 0 && player.y != 0) {
			worldMap[player.x][player.y].type = 0;
			player.y -= player.speed;
		}
		else if(event.key.keysym.sym == SDLK_a && worldMap[player.x - player.speed][player.y].type == 0 && player.x != 0) {
			worldMap[player.x][player.y].type = 0;
			player.x -= player.speed;
		}
		else if(event.key.keysym.sym == SDLK_s && player.y != mapHeight && worldMap[player.x][player.y + player.speed].type == 0) {
			worldMap[player.x][player.y].type = 0;
			player.y += player.speed;
		}
		else if(event.key.keysym.sym == SDLK_d && player.x != mapWidth && worldMap[player.x + player.speed][player.y].type == 0) {
			worldMap[player.x][player.y].type = 0;
			player.x += player.speed;
		}
	}
}
