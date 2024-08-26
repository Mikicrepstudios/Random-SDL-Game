#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <string>

#include "addional.h"
#include "settings.h"

namespace player {
	void GameInfo(SDL_Renderer* renderer, game::SDL_Settings sdlSettings, game::Camera camera, game::Player player, SDL_Color color) {
		// Rects
		SDL_Rect fpsRect = {0, 0, 100, 20};

		SDL_Rect camScaleRect = {0, 20, 100, 20};
		SDL_Rect camXRect = {0, 50, 100, 20};
		SDL_Rect camYRect = {0, 70, 100, 20};

		SDL_Rect playerXRect = {0, 100, 100, 20};
		SDL_Rect playerYRect = {0, 120, 100, 20};

		// Text processing
		std::string fpsText = "Fps: " + std::to_string(sdlSettings.fps);
		std::string camScaleText = "Camscale: " + std::to_string(camera.scale);

		std::string camXText = "Cam X: " + std::to_string(abs(camera.offSetX));
		std::string camYText = "Cam Y: " + std::to_string(abs(camera.offSetY));

		std::string playerXText = "Player X: " + std::to_string(player.x);
		std::string playerYText = "Player Y: " + std::to_string(player.y);

		// Show results
		draw::DrawText(renderer, sdlSettings.font, fpsRect, fpsText.c_str(), color);
		draw::DrawText(renderer, sdlSettings.font, camScaleRect, camScaleText.c_str(), color);

		draw::DrawText(renderer, sdlSettings.font, camXRect, camXText.c_str(), color);
		draw::DrawText(renderer, sdlSettings.font, camYRect, camYText.c_str(), color);

		draw::DrawText(renderer, sdlSettings.font, playerXRect, playerXText.c_str(), color);
		draw::DrawText(renderer, sdlSettings.font, playerYRect, playerYText.c_str(), color);
	}
}
