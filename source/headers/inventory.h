#ifndef INVENTORY_H
#define INVENTORY_H

#include <SDL2/SDL.h>

#include "presets.h"

namespace inventory {
    struct rects {
        SDL_Rect colorRect = {};
        SDL_Rect colorRectb = {};
        SDL_Rect colorTextRect = {};
        SDL_Rect bgColorRect = {};
        SDL_Rect bgColorRectb = {};
        SDL_Rect bgColorTextRect = {};
        SDL_Rect playerColorRect = {};
        SDL_Rect playerColorRectb = {};
        SDL_Rect playerColorTextRect = {};
        SDL_Rect previewRect = {};
        SDL_Rect previewRectb = {};
        SDL_Rect previewTextRect = {};
        SDL_Rect bgGameplay = {};
        SDL_Rect gameplayTextRect = {};
        SDL_Rect camTpRect = {};
        SDL_Rect playerTpRect = {};
        SDL_Rect bgGame = {};
        SDL_Rect gameTextRect = {};
        SDL_Rect saveRect = {};
        SDL_Rect loadRect = {};
        SDL_Rect gameInfoRect = {};
        SDL_Rect exitRect = {};
        SDL_Rect presetRect = {};
        SDL_Rect presetTextRect = {};
        SDL_Rect selCLPresetRect = {};
        SDL_Rect selCRPresetRect = {};
        SDL_Rect presetTitleRect = {};
    };

    rects InitRects(game::SDL_Settings sdlSettings);

    // Handle pressing E
    extern void Event(SDL_Event event, game::Settings &settings);

    // Handle mouse clicks
	extern void Chooser(SDL_Renderer* renderer, SDL_Event event, inventory::rects rects, game::SDL_Settings &sdlSettings, game::Settings &settings, game::Map &map, game::Player &player, game::Camera &camera, game::Preset preset[10]);

    // Draw UI
    extern void Overlay(SDL_Renderer* renderer, TTF_Font* font, inventory::rects rects, bool inventory, bool gameInfo, int blockColor, int bgColor,  int playerColor, int width, int height, int mouseX, int mouseY, int preset, game::SDL_Settings sdlSettings, game::Settings settings);
}

#endif