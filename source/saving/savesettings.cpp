#include <fstream>
#include <iostream>

#include "game.h"
#include "settings.h"

namespace files {
	void SaveSettings(game::Settings settings, game::Player player, game::Camera cam) {
		/**
		 * @brief This function saves game settings to file
		 * @param settings Game settings
		 * @param player Game player
		 * @param cam Game camera
		 */
		
		std::ofstream settingsFile("save/settings.msave"); // Open file

		// Save settings
		if(settingsFile.is_open()) {
			settingsFile << 
				   settings.blockColor << " "
				<< settings.bgColor << " "

				<< player.x << " "
				<< player.y << " "
				<< settings.playerColor << " "

				<< cam.offSetX << " "
				<< cam.offSetY << " "
				<< cam.scale << " ";
			settingsFile.close();
		}
	}

	void LoadSettings(game::Settings &settings, game::Player &player, game::Camera &cam, game::Preset preset[10]) {
		/**
		 * @brief This function loads saved game settings
		 * @param settings Game settings
		 * @param player Game player
		 * @param cam Game camera
		 * @param preset Game presets
		 */
		
		std::ifstream settingsFile("save/settings.msave"); // Open file

		// Load settings
		if(settingsFile.is_open()) {
			settingsFile >> 
					preset[settings.curPreset].blockColor >>
					settings.bgColor >>

					player.x >>
					player.y >>
					settings.playerColor >>

					cam.offSetX >>
					cam.offSetY >>
					cam.scale;
			settingsFile.close();
		}
	}
}
