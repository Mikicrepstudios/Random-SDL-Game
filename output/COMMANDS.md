# Random SDL Game - CLI

## Terminal Mode (F9)

Press **F9** to open Terminal Mode. While Terminal Mode is active, you cannot interact with the game window. A red pixel will appear in the top-left corner of the screen to indicate that Terminal Mode is enabled.

After you enter a command, Terminal Mode will automatically disable itself—unless you have explicitly enabled persistent Terminal Mode (details below).

## Command list

Syntax:
command-name [required] <optional>

Arguments:
- [required]    Required argument.
- <optional>    Optional argument.

Example:
> echo [text]

### Player commands
**setbgcolor [colorid]**
- This command sets background color of game to specified [colorid] (1-32)

**setblockcolor [colorid]**
- This command sets color of placed blocks to specified [colorid] (1-32)

**setblocktexture [textureid]**
- This command sets texture of placed blocks to specified [textureid] (1-16)

**setplayercolor [colorid]**
- This command sets player color to specified [colorid] (1-32)

**tpcam [x] [y]**
- This command teleports camera to specified coordinates

**tpplayer [x] [y]**
- This command teleports player to specified coordinates


### Utility commands
**echo [text]**
- This command prints [text] to terminal

**getbgcolor**
- This command prints game background color

**getblockcolor [x] [y]**
- This command prints color of block on specified coordinates

**getblocktexture [x] [y]**
- This command prints textureid of block on specified coordinates

**getblocktype [x] [y]**
- This command prints type of block on specified coordinates

**getcampos**
- This command prints camera position

**getplayercolor**
- This command prints color of player

**getplayerpos**
- This command prints player position

**isblocksolid [x] [y]**
- This command checks and prints is block on specified coordinates solid

**terminalmode [state]**
- This command sets terminal mode to state (on/true/1 or off/false/0)


### World commands
**destroy [x] [y]**
- This command destroys block on specified coordinates

**place [x] [y]**
- This command places block using your selected settings on specified coordinates