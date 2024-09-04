## Added:
-- Build 56
- Added namespace mouse
- Game icon

-- Build 57
- Added namespace colorpicker
- Added bool cheats and int cheatsId to namespace settings
- Added SDL_Event in game::SDL_Settings
- Added more stuff to gameInfo

-- Build 58
- Added namespace hud
- Added cliInput bool to game::sdlSettings
- Added f9 shortcut for very very experimental testing scripting terminal
- Added echo command (UNFINISHED)

-- Build 59
- Finished echo command
- Added args and argsCount variable
- Added place command

-- Build 60
- Added blockColor argument to place command
- Place command now outputs result

## Removed:
-- Build 56
- Shortened function arguments

-- Build 57
- Removed namespace overlay and overlay.h
- Removed struct game::cheats
- Shortened function arguments

-- Build 58
- Removed player::initGameInfoText() function
- Shortened function arguments

-- Build 60
- Shortened function arguments

-- Build 61
- Shortened function arguments (finished finnaly)

-- Build 62
- Added curPreset in gameInfo

## Changed/Fixed:
-- Build 56
- Renamed MouseInvChooser() to Chooser(), InventoryEvent() to Event(), InventoryOverlay() to Overlay()
- Moved all inventory functions to inventory namespace
- Moved overlay::Mouse() to mouse::Overlay() and player::MouseEvent() to mouse::Event()

-- Build 57
- Moved overlay:ColorPicker() to colorpicker:Overlay() and player::ColorPickerEvent() to colorpicker:Event()
- Renamed hud.cpp to gameinfo.cpp
- Rewrote gameInfo code

-- Build 58
- Renamed game::settings camera to game::settings cam
- Renamed addional.h to draw.h
- Moved player::GameInfo() function to hud::GameInfo()

-- Build 61
- "Whole" inventory mess is finnaly over

-- Build 62
- SDL_Window is now defined bit nicer so now main.cpp is fully readable without scrolling sideways

## Notes:


###### Changelogs are cleared every release