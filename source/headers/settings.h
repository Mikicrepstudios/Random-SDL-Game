#pragma once

#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "block.h"

namespace game {
    struct Game {
        bool cliInput = false;

        int curHoverX = 0;
        int curHoverY = 0;
    };

    struct Settings {
        // Player settings
	    int curPreset = 0;
        int blockColor = 1; // Updates every frame
        int blockTextureId = 0;
        int bgColor = 1;
        int playerColor = 6; // Write here to update

        // Bools
        bool canPlayerPlace = true;
        bool placeSolidBlocks = true;

        // Event settings
        bool inventory = false;

        // Pickers
        int colorPickerId = 0;
        bool colorPicker = false;
        bool texturePicker = false;

        // Dialogues
        int dialogueId = 0;
        bool dialogue = false;

        // Cheats
        int cheatsId = 0;
        bool cheats = false;

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
	    bool highlight = false; // Updates every frame
    };
}