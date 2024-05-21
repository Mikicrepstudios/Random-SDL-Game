#include <fstream>
#include <iostream>

namespace files {
	void SaveSettings(int playerX, int playerY, int camOffSetX, int camOffSetY, int camScale, int playerColor, int blockColor, int bgColor) {
		std::ofstream settingsFile("save/settings.msave"); // Open file

		// Save settings
		if(settingsFile.is_open()) {
			settingsFile << playerX << " "
				<< playerY << " "
				<< camOffSetX << " "
				<< camOffSetY << " "
				<< camScale << " "
				<< playerColor << " "
				<< blockColor << " "
				<< bgColor << " ";
			settingsFile.close();
		}
	}

	void LoadSettings(int &playerX, int &playerY, int &camOffSetX, int &camOffSetY, int &camScale, int &playerColor, int &blockColor, int &bgColor) {
		std::ifstream settingsFile("save/settings.msave"); // Open file

		// Load settings
		if(settingsFile.is_open()) {
			settingsFile >> playerX >>
					playerY >>
					camOffSetX >>
					camOffSetY >>
					camScale >>
					playerColor >>
					blockColor >>
					bgColor;
			settingsFile.close();
		}
	}
}
