#pragma once

#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "block.h"

namespace game {
    struct SDL_Settings {
        TTF_Font* font;
        SDL_Color textColor = {255, 255, 255};
	    SDL_Color altTextColor = {0, 0, 0};

        bool running = true;
        bool isMouseDown = false;

        int fps = 60;
        int width = 1280;
        int height = 800;

        int mouseX = 0;
        int mouseY = 0;
        int curHoverX = 0;
        int curHoverY = 0;
    };

    struct Settings {
        // Player settings
	    int curPreset = 0;
        int blockColor = 0; // Updates every frame
        int bgColor = 1;
        int playerColor = 6; // Write here to update

        // Event settings
        bool inventory = false;
	    bool colorPickerTool = false;

        // Color pickers
        bool colorPick = false;
        bool bgColorPick = false;
	    bool playerColorPick = false;

        // Addional settings
	    bool gameInfo = false;
    };

    struct Map {
        int width = 250 - 1;
        int height = 250 - 1;
        Block map[250][250] = {};
    };

    struct Player {
        // Pos
        int x = 0;
        int y = 0;

        // Addional
        int color = 6;
        int speed = 1;
    };

    struct Camera {
        // Visual
        int scale = 50;
        int offSetX = 0;
        int offSetY = 0;

        // Effects
	    bool highlight = false;
    };

    struct Dialogues {
        bool exitDialogue = false;
	    bool clearDialogue = false;
    };

    struct Cheats {
        bool camTp = false;
        bool playerTp = false;
    };
}