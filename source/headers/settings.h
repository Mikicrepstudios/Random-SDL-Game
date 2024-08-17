#pragma once

namespace game {
    struct Settings {
        // Player settings
	    int curPreset = 0;
        int bgColor = 1;

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
}