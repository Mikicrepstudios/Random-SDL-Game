#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <stdio.h>

#include "mf/colors.h"
#include "mf/core.h"
#include "mf/graphics.h"

#include "block.h"
#include "cheats.h"
#include "commands.h"
#include "debug.h"
#include "dialogues.h"
#include "files.h"
#include "game.h"
#include "gui.h"
#include "inventory.h"
#include "settings.h"
#include "textures.h"

int main(int argc, char **argv) {
  bool running = true;
  core::printver(3); // Print Mikicrep Framework version
  std::cout << "-------Random SDL Game----------" << std::endl
            << "-------Ver: D E V---------------" << std::endl
            << "Copyright Mikicrep Studios 2023-2026" << std::endl;

  // Main stuff
  const std::string title = "Mikicrep Framework"; // Latest release 2.0.1
  core::MF_Window window = {};
  SDL_Event event = {};
  bool debug = false;

  // Create window
  if (debug)
    std::cout << "Creating window" << std::endl;
  if (core::InitWindow(window, title, 1280, 800) == false)
    running = false;

  // Structs
  if (debug)
    std::cout << "Defining structs" << std::endl;
  game::Game game(running);
  auto &settings = game.settings;
  auto &cam = game.cam;
  auto &map = game.map;
  auto &player = game.player;

  // Do Main Menu
  if (debug)
    std::cout << "Running Main Menu" << std::endl;
  game::MainMenu(window, game, running);

  // Preset stuff
  if (debug)
    std::cout << "Defining presets" << std::endl;
  game::Preset preset[10] = {};
  preset[0].blockColor = 2;
  preset[0].textureId = 0;

  // Prepare game
  // Initilize structs
  if (debug)
    std::cout << "Start preparing game : Initilizing structs" << std::endl;
  dialogues::rects dialoguesRects = dialogues::InitRects(window);

  // Inv rects
  inventory::MenuRects inventoryMenuRects = inventory::InitMenuRects(window);
  inventory::ColorRects inventoryColorRects = inventory::InitColorRects(window);
  inventory::DecalRects inventoryDecalRects = inventory::InitDecalRects(window);
  inventory::GameplayRects inventoryGameplayRects =
      inventory::InitGameplayRects(window);
  inventory::GameRects inventoryGameRects = inventory::InitGameRects(window);
  inventory::OtherRects inventoryOtherRects = inventory::InitOtherRects(window);

  // Textures
  if (debug)
    std::cout << "Init textures" << std::endl;
  textures::BlockTextures blockTextures[32] = {};
  textures::initBlocks(window, blockTextures);

  if (debug)
    std::cout << "Init icon.png" << std::endl;

  // Adding icon to window
  SDL_Surface *iconSurface = IMG_Load("icon.png");
  SDL_SetWindowIcon(window.window, iconSurface);
  SDL_FreeSurface(iconSurface);

  if (debug)
    std::cout << "Init assets/background.png" << std::endl;

  // Load custom bg
  SDL_Surface *backgroundSurface = IMG_Load("assets/background.png");
  SDL_Texture *backgroundTexture =
      SDL_CreateTextureFromSurface(window.renderer, backgroundSurface);
  SDL_FreeSurface(backgroundSurface);

  game::ClearMap(game);
  if (game.menuLoad)
    files::LoadGame(game);

  if (debug)
    std::cout << "Start running loop" << std::endl;

  while (running) {
    // Prepare next frame
    core::BeginMouseFrame(window);
    game.curHoverX = window.mouse.x / cam.scale;
    game.curHoverY = window.mouse.y / cam.scale;

    // Update vars
    game::UpdateVars(game, preset);

    // Check for events
    while (SDL_PollEvent(&event) != 0) {
      // Handle window events
      window.event = event;
      if (event.type == SDL_MOUSEBUTTONDOWN ||
          event.type == SDL_MOUSEBUTTONUP) {
        core::HandleMouseEvent(window);
      }

      switch (event.type) {
      case SDL_QUIT:
        // Quit game
        running = false;
        break;

      case SDL_WINDOWEVENT:
        switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED:
          // Handle resizing window
          window.width = event.window.data1;
          window.height = event.window.data2;

          // Update rects
          dialoguesRects = dialogues::InitRects(window);

          // Inv rects
          inventoryMenuRects = inventory::InitMenuRects(window);
          inventoryColorRects = inventory::InitColorRects(window);
          inventoryDecalRects = inventory::InitDecalRects(window);
          inventoryGameplayRects = inventory::InitGameplayRects(window);
          inventoryGameRects = inventory::InitGameRects(window);
          inventoryOtherRects = inventory::InitOtherRects(window);
          break;
        }
        break;

      case SDL_MOUSEBUTTONDOWN:
        // Cheats
        if (settings.cheats) {
          int cheatsResult = 0;

          switch (settings.cheatsId) {
          case 1:
            cheatsResult = cheats::CamTp(window, game);
            break;
          case 2:
            cheatsResult = cheats::PlayerTp(window, game);
            break;
          }

          if (cheatsResult == 1)
            settings.cheats = false;
        }

        // Inventory buttons click event
        if (settings.inventory)
          inventory::Chooser(window, game, preset, inventoryMenuRects,
                             inventoryColorRects, inventoryDecalRects,
                             inventoryGameplayRects, inventoryGameRects,
                             inventoryOtherRects);

        if (settings.canPlayerPlace == true)
          game::MouseEvent(window, game, preset); // For click placing

        break;
      case SDL_TEXTINPUT:
        if (window.typingVariable)
          window.typingVariable->append(event.text.text);
        break;

      case SDL_KEYDOWN:
        // Handle keyboard presses
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          // Quit game
          if (window.isTypingActive) {
            window.isTypingActive = false;
            window.typingVariable = nullptr;
            SDL_StopTextInput();
          }

          if (!settings.inventory) {
            settings.dialogueId = 1;
            settings.dialogue = !settings.dialogue;
          }

          settings.colorPicker = false;
          settings.inventory = false;
          break;

        case SDLK_BACKSPACE:
          // Next line is for input fields, if you want to add another thing to
          // backspace, do it below if statement line
          if (window.isTypingActive && window.typingVariable &&
              !window.typingVariable->empty())
            window.typingVariable->pop_back();
          break;

        case SDLK_RETURN:
          // Next line is for input fields, if you want to add another thing to
          // enter, do it below if statement line
          if (window.isTypingActive) {
            window.isTypingActive = false;
            window.typingVariable = nullptr;
            SDL_StopTextInput();
          }
          break;

        case SDLK_F3:
          settings.gameInfo = !settings.gameInfo;
          break;

        case SDLK_F9:
          game.cliInput = true;
          break;

        case SDLK_F11:
          // Window fullscreening
          switch (window.fullscreen) {
          case true:
            SDL_SetWindowFullscreen(window.window, 0);
            window.fullscreen = false;
            break;
          case false:
            SDL_SetWindowFullscreen(window.window, SDL_WINDOW_FULLSCREEN);
            window.fullscreen = true;
            break;
          }

        default:
          if (!window.isTypingActive) {
            // Preset chooser
            game::PresetChooser(event, settings.curPreset);

            // Player movement
            game::PlayerMovement(event, game);

            // Inventory
            inventory::Event(event, game);

            // Camera
            game::CameraControls(window, game);

            switch (event.key.keysym.sym) {
            case SDLK_q:
              settings.cheatsId = 3;
              settings.cheats = !settings.cheats;
              break;

            case SDLK_c:
              settings.dialogueId = 2;
              settings.dialogue = !settings.dialogue;
              break;
            }
          }
          break;
        }
        break;

      default:
        if (settings.canPlayerPlace == true)
          game::MouseEvent(window, game, preset); // For drag placing
        break;
      }
    }
    core::EndMouseFrame(window);

    // Set BG color to new color
    draw::SetDrawColor(window.renderer, colors::colorID[settings.bgColor - 1]);
    SDL_RenderClear(window.renderer);
    // ... or if there is custom bg
    SDL_Rect backgroundRect = {0, 0, window.width, window.height};
    SDL_RenderCopy(window.renderer, backgroundTexture, NULL, &backgroundRect);

    // Pre logic
    // Prepare player
    map.map[player.x][player.y].type = 1;

    // Draw map
    game::RenderMap(window, game, blockTextures);

    // Overlays
    inventory::Overlay(window, game, inventoryMenuRects, inventoryColorRects,
                       inventoryDecalRects, inventoryGameplayRects,
                       inventoryGameRects, inventoryOtherRects, blockTextures);

    game::MouseOverlay(window, game);

    // Game info
    if (settings.gameInfo)
      gui::GameInfo(window, game);

    dialogues::CallDialogue(window, game, dialoguesRects);

    // Cli Input
    if (game.cliInput || game.terminalmode) {
      std::string command = "";
      draw::SetDrawColor(window.renderer,
                         colors::colorID[settings.bgColor - 1]);
      draw::DrawRect(window.renderer, {0, 0, 25, 25}, colors::red);
      SDL_RenderPresent(window.renderer);
      std::getline(std::cin, command);
      commands::Executor(command, window, game, preset);
      game.cliInput = false;
    }

    // Show results
    SDL_RenderPresent(window.renderer);

    // Timer
    SDL_Delay(1000 / window.fps);
  }

  core::Exit(window);
  return 0;
}
