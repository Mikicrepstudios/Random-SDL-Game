#include <string>

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"

#include "settings.h"

namespace gui {
	void GameInfo(core::MF_Window &window, game::Settings settings, game::Camera cam, game::Player player) {
		/**
		 * @brief This function draws GameInfo HUD
		 * @param window Game window
		 * @param settings Game settings
		 * @param cam Game camera
		 * @param player Game player
		 */
		
		MF_Color color = {};
		// If background is white then set text color to black otherwise its white
		if(settings.bgColor == 32)
			color = colors::black;
		else
			color = colors::white;
		
		std::string gameInfoTexts[16] = {
			"Width: " + std::to_string(window.width),
			"Height: " + std::to_string(window.height),
			"Fps: " + std::to_string(window.fps),
			"", // Empty string for skipped line
			"Cur preset: " + std::to_string(settings.curPreset + 1),
			"Block color: " + std::to_string(settings.blockColor),
			"BG color: " + std::to_string(settings.bgColor),
			"", // Empty string for skipped line
			"Cam X: " + std::to_string(cam.offSetX),
			"Cam Y: " + std::to_string(cam.offSetY),
			"Camscale: " + std::to_string(cam.scale),
			"", // Empty string for skipped line
			"Player X: " + std::to_string(player.x),
			"Player Y: " + std::to_string(player.y),
			"Player color: " + std::to_string(settings.playerColor)
		};
		// Draw gameInfo hud
		for(int i = 0; i <= 13; i++) {
			SDL_Rect curRect = {0, i * 20, 100, 20};
			draw::DrawText(window.renderer, window.font, curRect, gameInfoTexts[i].c_str(), color);
		}
	}
}
