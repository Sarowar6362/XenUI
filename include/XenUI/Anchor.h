
#ifndef ANCHOR_H
#define ANCHOR_H
#include <SDL2/SDL.h>
namespace XenUI {
    
enum class Anchor {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CENTER,
    TOP_CENTER,
    BOTTOM_CENTER,
    CENTER_LEFT,
    CENTER_RIGHT
};

// Converts anchor + offset into actual screen coordinates
SDL_Point ResolveAnchorPosition(Anchor anchor, int offsetX, int offsetY, int width, int height);
}
#endif

