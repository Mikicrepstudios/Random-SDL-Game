#include "SDL2/SDL.h"

#include "block.h"
#include "settings.h"

namespace player {
	void Movement(SDL_Event event, game::Map &map, game::Player &player) {
		if(event.key.keysym.sym == SDLK_w && map.map[player.x][player.y - player.speed].type == 0 && player.y != 0) {
			map.map[player.x][player.y].type = 0;
			player.y -= player.speed;
		}
		else if(event.key.keysym.sym == SDLK_a && map.map[player.x - player.speed][player.y].type == 0 && player.x != 0) {
			map.map[player.x][player.y].type = 0;
			player.x -= player.speed;
		}
		else if(event.key.keysym.sym == SDLK_s && player.y != map.height && map.map[player.x][player.y + player.speed].type == 0) {
			map.map[player.x][player.y].type = 0;
			player.y += player.speed;
		}
		else if(event.key.keysym.sym == SDLK_d && player.x != map.width && map.map[player.x + player.speed][player.y].type == 0) {
			map.map[player.x][player.y].type = 0;
			player.x += player.speed;
		}
	}
}
