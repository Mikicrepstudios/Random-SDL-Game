#pragma once

#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"

#include "block.h"

// Already documented in game.h
namespace game {
    struct Game {
        bool &running;

        // Menu vars
        bool menuLoad = false; // Load game on start - changed in mainmenu.cpp
        std::string savePath = "default"; // Default save name

        bool terminalmode = false;
        bool cliInput = false;

        int curHoverX = 0;
        int curHoverY = 0;

        struct Settings {
            // Player settings
            int curPreset = 0;
            int blockColor = 1; // Updates every frame
            int blockTextureId = 0; // Updates every frame
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
        } settings;

        struct Map {
            int width = 250; // Theese have to be one block less because arrays start with 0
            int height = 250;

            std::vector<std::vector<Block>> map;

            Map() {
                // Resize to 250 rows
                map.resize(width);
                // Resize each row to 250 columns
                for (int i = 0; i < width; i++) {
                    map[i].resize(height);
                }
            }

            //Block map[250][250] = {}; // Old agony
        } map;

        struct Player {
            // Pos
            int x = 0;
            int y = 0;

            // Addional
            int color = 6; // Updates every frame
            int speed = 1;
        } player;

        struct Camera {
            // Visual
            int scale = 50;
            int offSetX = 0;
            int offSetY = 0;

            // Effects
            bool highlight = false; // Updates every frame
        } cam;

        Game(bool &r) : running(r) {};
    };
}