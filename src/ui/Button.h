// Button.h
#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <string>
#include <functional>
#include "../graphics/sdl2/TextRenderer.h"
#include "../utils/Anchor.h"
#include "../utils/Position.h" // *** ADDED ***
#include <iostream>

// Define ButtonStyle structure (keep as is)
struct ButtonStyle {
    SDL_Color bgColor = {100, 100, 100, 255};
    SDL_Color textColor = {255, 255, 255, 255};
    bool drawBackground = true;
    bool drawBorder = true;
    int paddingX = 10;
    int paddingY = 5;
};

class Button {
public:
    // === RETAINED MODE ===
    // *** MODIFIED Constructor signature ***
    Button(const std::string& text,
           const XenUI::PositionParams& posParams, // Use PositionParams
           ButtonStyle style,
           std::function<void()> onClick);

    void draw(SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& event);
    void recalculatePosition();

private:
    std::string m_text;
    XenUI::PositionParams m_posParams; // *** ADDED: Store positioning intent ***
    // Remove m_anchor, m_relativeOffsetX, m_relativeOffsetY - replaced by m_posParams
    // Keep calculated values:
    int m_posX;
    int m_posY;
    int m_width;
    int m_height;
    ButtonStyle m_style;
    std::function<void()> m_onClick;
    bool m_hovered;
};

// === IMMEDIATE MODE BUTTON API ===
namespace XenUI {
    // *** MODIFIED: Single signature using PositionParams ***
    bool Button(const std::string& id, const std::string& text,
                const PositionParams& posParams, // Use PositionParams
                ButtonStyle style = {});
} // namespace XenUI

#endif // BUTTON_H