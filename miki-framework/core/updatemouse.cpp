#include "mf/core.h"

namespace core {
    // Call this ONCE at start of frame
    void BeginMouseFrame(core::MF_Window &window) {
        // Get mouse position
        SDL_GetMouseState(&window.mouse.x, &window.mouse.y);
        
        // Reset pressed state
        window.mouse.isPressed = false;
        
        // DO NOT touch wasDown here!
        // wasDown should stay as last frame's state
    }
    
    // Call this for EACH mouse event
    void HandleMouseEvent(core::MF_Window &window) {
        if(window.event.type == SDL_MOUSEBUTTONDOWN) {
            window.mouse.isDown = true;
        } else if(window.event.type == SDL_MOUSEBUTTONUP) {
            window.mouse.isDown = false;
        }
    }
    
    // Call this ONCE after processing ALL events
    void EndMouseFrame(core::MF_Window &window) {
        // Calculate pressed: is mouse down NOW but wasn't down LAST FRAME?
        window.mouse.isPressed = window.mouse.isDown && !window.mouse.wasDown;
        
        // Store current state for NEXT FRAME
        window.mouse.wasDown = window.mouse.isDown;
    }
}