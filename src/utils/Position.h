// src/utils/Position.h
#ifndef XENUI_POSITION_H
#define XENUI_POSITION_H

#include "Anchor.h"     // Include your Anchor enum definition
#include "WindowUtil.h" // For ResolveAnchorPosition dependency
#include <SDL2/SDL.h>

namespace XenUI {

    // Forward declare ResolveAnchorPosition if its definition is in Anchor.cpp
    // If ResolveAnchorPosition is defined inline in Anchor.h, this is not needed.
    SDL_Point ResolveAnchorPosition(Anchor anchor, int offsetX, int offsetY, int width, int height);

    enum class PositionMode {
        ABSOLUTE,  // Position is defined by direct x, y
        ANCHORED   // Position is relative to an anchor point
    };

    struct PositionParams {
        PositionMode mode = PositionMode::ABSOLUTE; // Default to absolute

        // --- Absolute ---
        int x = 0;
        int y = 0;

        // --- Anchored ---
        Anchor anchor = Anchor::TOP_LEFT; // Default anchor
        int relOffsetX = 0;
        int relOffsetY = 0;

        // --- Convenience Static "Constructors" ---
        static PositionParams Absolute(int absX, int absY) {
            PositionParams p;
            p.mode = PositionMode::ABSOLUTE;
            p.x = absX;
            p.y = absY;
            return p;
        }

        static PositionParams Anchored(Anchor anch, int rX = 0, int rY = 0) {
            PositionParams p;
            p.mode = PositionMode::ANCHORED;
            p.anchor = anch;
            p.relOffsetX = rX;
            p.relOffsetY = rY;
            return p;
        }
    };

    // --- Central Calculation Function ---
    // Calculates the final top-left screen coordinate based on parameters and element size
    // Defined inline for potential performance benefits, but could be in a .cpp if preferred.
    inline SDL_Point CalculateFinalPosition(const PositionParams& params, int elementWidth, int elementHeight) {
        if (params.mode == PositionMode::ABSOLUTE) {
            return { params.x, params.y };
        } else { // ANCHORED
            // Call the existing anchor resolution function
            return ResolveAnchorPosition(params.anchor, params.relOffsetX, params.relOffsetY, elementWidth, elementHeight);
        }
    }

} // namespace XenUI

#endif // XENUI_POSITION_H