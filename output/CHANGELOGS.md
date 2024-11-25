# GAME HAS BEEN PORTED TO MIKICREP FRAMEWORK (Build 78-80)

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

-- Build 62
- Added curPreset in gameInfo

-- Build 64
- Added canPlayerPlace bool

-- Build 65
- Added colorPickerTool to cheats

-- Build 67
- Added getblock command

-- Build 68
- Added getblocktype command
- FINNALY Added non collision block WHICH WAS ON TODO SINCE BUILD 6
- Added mapsolids.msave
- Added isblocksolid command

-- Build 69
- Added extra if for cheats

-- Build 71
- Added getbgcolor command

-- Build 72
- Added destroy command
- Added setblockcolor command

-- Build 73
- Added setbgcolor and setplayercolor commands
- Added detection if player sets color outside color range

-- Build 74
- Brick, placeholder texture
- Textures (TESTING, UNFINISHED, UNOPTIMIZED)

-- Build 75
- Added block.texture
- Added blockTextureId to game::Settings
- Grass, stone texture
- Added draw::DrawRectTexture()
- BLOCK TEXTURES (mostly) FINISHED - WAS ON TODO SINCE BUILD 15
- Added maptextures.msave

-- Build 76
- Added debugging statements

-- Build 77
- Added lots of textures

-- Build 81
- Texture has been added on top of preview rect

-- Build 84
- Added cactus texture

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

-- Build 65
- Removed bool colorPickerTool

-- Build 68
- Removed block struct function

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

-- Build 63
- Changed player::PlayerMovement() to player::Movement()

-- Build 64
- gameInfoTexts[16] is now defined nicer
- Fixed bug no. 4

-- Build 65
- Changed a bit mouse::Event() function

-- Build 66
- Split inventory.cpp in multiple files

-- Build 68
- Renamed getblock command to getblockcolor
- Rewrote player::Movement()
- Changed a bit player::Event()
- Player is now overlay
- Greatly improved savemap.cpp code
- Renamed map.msave to maptype.msave // DO THIS TO MIRATE SAVE
- Renamed map2.masave to mapcolors.msave // DO THIS TO MIGRATE SAVE
- Removed some artifacts from default map

-- Build 70
- Cheats functions are now int, disabling cheats is now handled outside of function
- Cheats calling is now in switch statement

-- Build 73
- Fixed demo map collisions
- Fixed loading block color from save

-- Build 75
- Block texture is not drawn on top of player anymore
- Optimized texture CPU usage (by not rendering air texture)
- Fixed bug No. 4
- Fixed issue where when you close dialogue, its buttons are still detected

-- Build 76
- Added checks to draw::DrawRect to colorId, it used to access invalid memory
- WINFIX WENT UNSUCCESSFULY

-- Build 81
- Renamed customize folder to assets
- Texture loading now uses Mikicrep Framework function
- Fixed Preview rect color

## Notes:


###### Changelogs are cleared every release