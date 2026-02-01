#ifndef INVENTORY_H
#define INVENTORY_H

#include "SDL.h"

#include "mf/core.h"

#include "game.h"
#include "settings.h"
#include "textures.h"

/**
 * @brief Namespace that contains all inventory related stuff
 */
namespace inventory {
    struct MenuRects {
        SDL_Rect previewRect = {};
        SDL_Rect previewRectb = {};
        SDL_Rect previewTextRect = {};

        SDL_Rect bgGameplay = {};
        SDL_Rect gameplayTextRect = {};
        SDL_Rect bgGame = {};
        SDL_Rect gameTextRect = {};
    };
    struct ColorRects {
        SDL_Rect colorRect = {};
        SDL_Rect colorRectb = {};
        SDL_Rect colorTextRect = {};

        SDL_Rect bgColorRect = {};
        SDL_Rect bgColorRectb = {};
        SDL_Rect bgColorTextRect = {};

        SDL_Rect playerColorRect = {};
        SDL_Rect playerColorRectb = {};
        SDL_Rect playerColorTextRect = {};
    };
    struct DecalRects {
        SDL_Rect textureIdRect = {};
        SDL_Rect textureIdRectb = {};
        SDL_Rect textureIdTextRect = {};
    };
    struct GameplayRects {
        SDL_Rect camTpRect = {};
        SDL_Rect playerTpRect = {};
    };
    struct GameRects {
        SDL_Rect saveRect = {};
        SDL_Rect loadRect = {};
        SDL_Rect gameInfoRect = {};
        SDL_Rect exitRect = {};
    };
    struct OtherRects {
        /*SDL_Rect textureColorRect = {};
        SDL_Rect textureColorRectb = {};
        SDL_Rect textureColorTextRect = {};*/

        SDL_Rect solidRect = {};
        SDL_Rect solidRectb = {};
        SDL_Rect solidTextRect = {};

        SDL_Rect presetRect = {};
        SDL_Rect presetTextRect = {};
        SDL_Rect presetTitleRect = {};
    };

    MenuRects InitMenuRects(core::MF_Window &window);
    ColorRects InitColorRects(core::MF_Window &window);
    DecalRects InitDecalRects(core::MF_Window &window);
    GameplayRects InitGameplayRects(core::MF_Window &window);
    GameRects InitGameRects(core::MF_Window &window);
    OtherRects InitOtherRects(core::MF_Window &window);

    // Handle pressing E
    void Event(SDL_Event event, game::Game &game);

    // Handle mouse clicks
	void Chooser(core::MF_Window &window, game::Game &game, game::Preset preset[10], inventory::MenuRects &menuRects, inventory::ColorRects &colorRects, inventory::DecalRects &decalRects, inventory::GameplayRects &gameplayRects, inventory::GameRects &gameRects, inventory::OtherRects &otherRects);

    // Draw UI
    void Overlay(core::MF_Window &window, game::Game &game, inventory::MenuRects &menuRects, inventory::ColorRects &colorRects, inventory::DecalRects &decalRects, inventory::GameplayRects &gameplayRects, inventory::GameRects &gameRects, inventory::OtherRects &otherRects, textures::BlockTextures blockTextures[32]);
}

#endif