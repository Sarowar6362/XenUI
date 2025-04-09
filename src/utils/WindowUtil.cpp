#include "WindowUtil.h"

static SDL_Window* g_window = nullptr;

namespace XenUI {

    // Sets the SDL_Window reference.
    void SetWindow(SDL_Window* window) {
        g_window = window;
    }

    // Returns the current window size (x = width, y = height).
    SDL_Point GetWindowSize() {
        if (g_window) {
            SDL_Point newSize;
            SDL_GetWindowSize(g_window, &newSize.x, &newSize.y);

            // Update only if the size has changed.
            static SDL_Point g_lastWindowSize = {800, 600}; // Default size
            if (newSize.x != g_lastWindowSize.x || newSize.y != g_lastWindowSize.y) {
                g_lastWindowSize = newSize;
            }
            return g_lastWindowSize;
        }

        return {800, 600}; // Fallback if window is not set
    }

}
