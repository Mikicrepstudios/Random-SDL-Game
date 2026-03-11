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
    int curPreset = 0;      // FLAG
    int blockColor = 1;     // Updates every frame
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
    float uiScale = 1;

    // Variables used for scaling elements
    float wSFactor = 1;
    float hSFactor = 1;
  } settings;

  struct Map {
    int width = 50;
    int height = 50;

    std::vector<std::vector<Block>> map;

    Map() {
      // Resize to width rows
      map.resize(width);
      // Resize each row to height columns
      for (int i = 0; i < width; i++) {
        map[i].resize(height);
      }
    }

    // Block map[250][250] = {}; // Old agony
  } map;

  struct Player {
    // Pos
    int x = 0;
    int y = 0;

    // Addional
    int color = 6; // Updates every frame
    int speed = 1;

    struct InventorySlot {
      int id = 0;     // Item ID
      int amount = 0; // Amount of specific item
    } inventory[10];

    const int inventorySize = 10;
    int curInventorySlot = 0;
  } player;

  struct Camera {
    // Visual
    int scale = 50;
    int offSetX = 0;
    int offSetY = 0;

    // Effects
    bool highlight = false; // Updates every frame
  } cam;

  struct Database {
    struct Items {
      const char *name = "";
      int id = 0;

      int color = 0;
      int texture = 0;

      bool isSolid = true;
    } items[32];
  } database;

  Game(bool &r) : running(r) {};
};
} // namespace game