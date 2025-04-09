// src/utils/Anchor.cpp
#include "Anchor.h"
#include "WindowUtil.h" // Provides XenUI::GetWindowSize()
#include <SDL2/SDL.h>   // Include SDL headers if needed by WindowUtil implementation or for SDL_Point
#include <iostream>     // For potential error logging

namespace XenUI {

    // Converts anchor + offset into actual screen coordinates
    SDL_Point ResolveAnchorPosition(Anchor anchor, int offsetX, int offsetY, int width, int height) {
        // --- FIX: Get ACTUAL window size ---
        SDL_Point windowSize = XenUI::GetWindowSize(); // Use the utility function!
        int windowWidth = windowSize.x;
        int windowHeight = windowSize.y;

        // Handle potential error case where window size might be zero (e.g., before fully initialized)
        if (windowWidth <= 0 || windowHeight <= 0) {
             // std::cerr << "Warning: ResolveAnchorPosition called with invalid window size ("
             //           << windowWidth << "x" << windowHeight << "). Returning raw offset." << std::endl;
             // Return raw offset or a default position? Returning offset might be less surprising.
             return { offsetX, offsetY };
        }
        // --- End FIX ---

        SDL_Point position = {0, 0}; // Initialize position

        switch (anchor) {
            case Anchor::TOP_LEFT:
                position.x = 0 + offsetX;
                position.y = 0 + offsetY;
                break;

            case Anchor::TOP_RIGHT:
                position.x = windowWidth - width + offsetX;
                position.y = 0 + offsetY;
                break;

            case Anchor::BOTTOM_LEFT:
                position.x = 0 + offsetX;
                position.y = windowHeight - height + offsetY;
                break;

            case Anchor::BOTTOM_RIGHT:
                position.x = windowWidth - width + offsetX;
                position.y = windowHeight - height + offsetY;
                break;

            case Anchor::CENTER:
                position.x = (windowWidth / 2) - (width / 2) + offsetX;
                position.y = (windowHeight / 2) - (height / 2) + offsetY;
                break;

            // --- FIX: Add missing anchor cases ---
            case Anchor::TOP_CENTER:
                position.x = (windowWidth / 2) - (width / 2) + offsetX;
                position.y = 0 + offsetY;
                break;

            case Anchor::BOTTOM_CENTER:
                position.x = (windowWidth / 2) - (width / 2) + offsetX;
                position.y = windowHeight - height + offsetY;
                break;

            case Anchor::CENTER_LEFT:
                position.x = 0 + offsetX;
                position.y = (windowHeight / 2) - (height / 2) + offsetY;
                break;

            case Anchor::CENTER_RIGHT:
                position.x = windowWidth - width + offsetX;
                position.y = (windowHeight / 2) - (height / 2) + offsetY;
                break;
            // --- End FIX ---

            default:
                // Default case could log an error or default to TOP_LEFT
                // std::cerr << "Warning: Unhandled anchor type in ResolveAnchorPosition. Defaulting to TOP_LEFT behavior." << std::endl;
                position.x = offsetX;
                position.y = offsetY;
                break;
        }

        return position;
    }

} // namespace XenUI