#include <fstream>
#include <iostream>

#include "settings.h"

namespace files {
	void SaveSettings(game::Settings settings, game::Player player, game::Camera camera) {
		std::ofstream settingsFile("save/settings.msave"); // Open file

		// Save settings
		if(settingsFile.is_open()) {
			settingsFile << 
				   settings.blockColor << " "
				<< settings.bgColor << " "

				<< player.x << " "
				<< player.y << " "
				<< settings.playerColor << " "

				<< camera.offSetX << " "
				<< camera.offSetY << " "
				<< camera.scale << " ";
			settingsFile.close();
		}
	}

	void LoadSettings(game::Settings &settings, game::Player &player, game::Camera &camera) {
		std::ifstream settingsFile("save/settings.msave"); // Open file

		// Load settings
		if(settingsFile.is_open()) {
			settingsFile >> 
					settings.blockColor >>
					settings.bgColor >>

					player.x >>
					player.y >>
					settings.playerColor >>

					camera.offSetX >>
					camera.offSetY >>
					camera.scale;
			settingsFile.close();
		}
	}
}
