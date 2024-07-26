# Random SDL Game
#### Most random game/game engine I made where I just put most random features.
- by Mikicrep

You can use it also as pixelart editor if you want.

## Compatibility
- Supports **linux** and **windows** (shh linux is given bigger love)
- Made for **ultra** low-end devices

## Running on Windows
If you get game by default then it wont run at all on windows.

You have to move all files from x86_64dll (for 64bit windows) or x86dll (for 32bit windows) folder to same place where is executable.

## Requirements
For linux compiling:

```pacman -S --needed base-devel sdl2 sdl2_ttf sdl2_image```

For windows compiling:

```pacman -S --needed mingw-w64-gcc```

Theese commands only works on arch based linux distros, for other ones you gotta find coresponding packages

## Compiling and running
1. Clone this repo
2. Open terminal and run `make`
- You can move `output` folder out of repo folder and delete repo folder
- You can delete .md and .dll files too but do not delete anything else
- Game will open automatically after successful compilation
### Windows x86_64 (64bit)
1. Clone this repo
2. Download `sdl2mingw.zip` [here](https://drive.google.com/drive/folders/1uVe7oKMzCTTUeMJuHL7vqK0O2FK_pIPs?usp=drive_link)
3. Extract `sdl2mingw.zip` in root folder of repo (make sure you put folder not contents of folder)
4. Open terminal and run `make windows`
5. Move contents from `x86_64dll` folder to output folder
- You can move `output` folder out of repo folder and delete repo folder
- You can delete .md files too but do not delete anything else
- Game will not open automatically after successful compilation
- Works under [WINE](https://www.winehq.org/) too
### Windows x86 (32bit)
1. Clone this repo
2. Download `sdl2mingw32.zip` [here](https://drive.google.com/drive/folders/1uVe7oKMzCTTUeMJuHL7vqK0O2FK_pIPs?usp=drive_link)
3. Extract `sdl2mingw32.zip` in root folder of repo (make sure you put folder not contents of folder)
4. Open terminal and run `make windows32`
5. Move contents from `x86dll` folder to output folder
- You can move `output` folder out of repo folder and delete repo folder
- You can delete .md files too but do not delete anything else
- Game will not open automatically after successful compilation
- Works under [WINE](https://www.winehq.org/) too

*Compiling is recommended unless you get game from [releases](https://github.com/Mikicrepstudios/Random-SDL-Game/releases) page*

## Documentation
Currently does not exist, will be [here](https://github.com/Mikicrepstudios/Random-SDL-Game/tree/docs)

## Bug reporting
First check is your bug listed in [BUGS.md](https://github.com/Mikicrepstudios/Random-SDL-Game/blob/master/output/BUGS.md)

If it isnt, you can report bugs using githubs [issues](https://github.com/Mikicrepstudios/Random-SDL-Game/issues) page, make sure you select "Bug report" template

## Suggestions
First check if your suggestion listed in [TODO.md](https://github.com/Mikicrepstudios/Random-SDL-Game/blob/master/TODO.md)

If it isnt, you can post suggestions in [issues](https://github.com/Mikicrepstudios/Random-SDL-Game/issues) page, make sure you select "Feedback request" template

## Contributing
Feel free to play with code, create pull requests or just give feedback on [Mikicrep Studios discord server](https://discord.gg/DdaGyeWp9U), it will really mean to me. If you want, as an contribution reward you will get your pfp or any pic you want to be added on our predefined save ;)

### Runs on potato.


