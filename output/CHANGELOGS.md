## [[2.2.0] Random SDL Game](https://miki.macakom.net/projects/rsg)
### New version just released

#### Added
- **Core**
    - Added support for multiple save slots

#### Removed
- (No removals in this update)

#### Changed
- **Dialogues**
    - You now have to click "No" button in dialogues instead of hovering over it

#### Code changes
- Dialogues got fully rewritten, now new ones can be added pretty easily, removed a lot of dialogue related lines in main.cpp and reduced amount of functions needed to be called in order for them to work
- Now every single struct in settings.h is moved inside of game.h struct, that allows it to easily access everything in only one function argument