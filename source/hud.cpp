#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <string>

#include "addional.h"

namespace player {
	void gameInfo(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, int fps, int camScale, int camOffSetX, int camOffSetY, int playerX, int playerY) {
		// Rects
		SDL_Rect fpsRect = {0, 0, 100, 20};

		SDL_Rect camScaleRect = {0, 20, 100, 20};
		SDL_Rect camXRect = {0, 50, 100, 20};
		SDL_Rect camYRect = {0, 70, 100, 20};

		SDL_Rect playerXRect = {0, 100, 100, 20};
		SDL_Rect playerYRect = {0, 120, 100, 20};

		// Text processing
		std::string fpsText = "Fps: " + std::to_string(fps);
		std::string camScaleText = "Camscale: " + std::to_string(camScale);

		std::string camXText = "Cam X: " + std::to_string(abs(camOffSetX));
		std::string camYText = "Cam Y: " + std::to_string(abs(camOffSetY));

		std::string playerXText = "Player X: " + std::to_string(playerX);
		std::string playerYText = "Player Y: " + std::to_string(playerY);

		// Show results
		draw::DrawText(renderer, font, fpsRect, fpsText.c_str(), color);
		draw::DrawText(renderer, font, camScaleRect, camScaleText.c_str(), color);

		draw::DrawText(renderer, font, camXRect, camXText.c_str(), color);
		draw::DrawText(renderer, font, camYRect, camYText.c_str(), color);

		draw::DrawText(renderer, font, playerXRect, playerXText.c_str(), color);
		draw::DrawText(renderer, font, playerYRect, playerYText.c_str(), color);
	}
}
