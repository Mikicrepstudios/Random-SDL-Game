#include "SDL.h"

#include "block.h"
#include "settings.h"

namespace game {
	void PlayerMovement(SDL_Event event, game::Game &game) {
		/**
		 * @brief This function controls player movement
		 * @param event SDL_Event
		 * @param map Game map
		 * @param player Game player
		 */

		auto& map = game.map;
		auto& player = game.player;
		
		// Make copy for smaller ifs
		Block curBlock = map.map[player.x][player.y];

		if(event.key.keysym.sym == SDLK_w && player.y != 0 && map.map[player.x][player.y - player.speed].isSolid == false) {
			if(curBlock.color == 0) map.map[player.x][player.y].type = 0;
			else map.map[player.x][player.y].type = 2;
			
			player.y -= player.speed;
		}
		else if(event.key.keysym.sym == SDLK_a && player.x != 0 && map.map[player.x - player.speed][player.y].isSolid == false) {
			if(curBlock.color == 0) map.map[player.x][player.y].type = 0;
			else map.map[player.x][player.y].type = 2;
			
			player.x -= player.speed;
		}
		else if(event.key.keysym.sym == SDLK_s && player.y != map.height && map.map[player.x][player.y + player.speed].isSolid == false) {
			if(curBlock.color == 0) map.map[player.x][player.y].type = 0;
			else map.map[player.x][player.y].type = 2;

			player.y += player.speed;
		}
		else if(event.key.keysym.sym == SDLK_d && player.x != map.width - 1 && map.map[player.x + player.speed][player.y].isSolid == false) {
			if(curBlock.color == 0) map.map[player.x][player.y].type = 0;
			else map.map[player.x][player.y].type = 2;
			
			player.x += player.speed;
		}
	}
}
