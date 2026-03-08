#include "mf/core.h"

namespace core {
// Call this ONCE at start of frame
void BeginMouseFrame(core::MF_Window &window) {
  // Store mouse x and y (also returns button mask)
  Uint32 buttons = SDL_GetMouseState(&window.mouse.x, &window.mouse.y);

  // Reset pressed for this frame
  window.mouse.isPressed = false;

  // Update left/right button state based on current mask. If no button is held,
  // both will be false. This handles the "set to false if none" requirement.
  window.mouse.leftButtonPressed = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
  window.mouse.rightButtonPressed =
      (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
}

// Call this for EACH mouse event
void HandleMouseEvent(core::MF_Window &window) {
  if (window.event.type == SDL_MOUSEBUTTONDOWN) {
    window.mouse.isDown = true;
    // mark which button was pressed
    if (window.event.button.button == SDL_BUTTON_LEFT) {
      window.mouse.leftButtonPressed = true;
    } else if (window.event.button.button == SDL_BUTTON_RIGHT) {
      window.mouse.rightButtonPressed = true;
    }
  } else if (window.event.type == SDL_MOUSEBUTTONUP) {
    window.mouse.isDown = false;
    // clear the appropriate button flag
    if (window.event.button.button == SDL_BUTTON_LEFT) {
      window.mouse.leftButtonPressed = false;
    } else if (window.event.button.button == SDL_BUTTON_RIGHT) {
      window.mouse.rightButtonPressed = false;
    }
  }
}

// Call this ONCE after processing ALL events
void EndMouseFrame(core::MF_Window &window) {
  window.mouse.isPressed = window.mouse.isDown && !window.mouse.wasDown;

  window.mouse.wasDown = window.mouse.isDown;
}
} // namespace core