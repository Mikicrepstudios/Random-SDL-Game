#pragma once

#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "block.h"

namespace game {
    struct SDL_Settings {
        SDL_Renderer* renderer = {};
        SDL_Event event = {};
        TTF_Font* font = {};
        SDL_Color textColor = {255, 255, 255};
	    SDL_Color altTextColor = {0, 0, 0};

        bool running = true;
        bool cliInput = false;
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

        // Bools
        bool canPlayerPlace = true;

        // Event settings
        bool inventory = false;
	    bool colorPickerTool = false; // todo remove (TURN INTO CHEATS)

        // Color pickers
        int colorPickerId = 0;
        bool colorPicker = false;

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
	    bool highlight = false;
    };
}