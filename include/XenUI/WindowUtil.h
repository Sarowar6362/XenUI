#pragma once
#include <SDL2/SDL.h>

namespace XenUI {
    // Sets the reference to the SDL_Window instance
    void SetWindow(SDL_Window* window);

    // Returns the current size of the window (x = width, y = height)
    SDL_Point GetWindowSize();
}
