#pragma once

namespace dialogues {
    int confirmDialogueEvent(SDL_Event event, int mouseX, int mouseY, int width, int height);
    int confirmDialogue(SDL_Renderer* renderer, TTF_Font* font, int width, int height, int mouseX, int mouseY, int textid);
}