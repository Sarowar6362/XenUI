#include "Label.h"
#include "../graphics/sdl2/TextRenderer.h"
#include "../utils/Anchor.h"
#include "../utils/WindowUtil.h"

#include <iostream>

// CHANGE: Updated Constructor
// In Label.cpp
Label::Label(const std::string& text,
    const XenUI::PositionParams& posParams,
    int fontSize,
    const SDL_Color& color)
: m_text(text),
m_posParams(posParams),
m_fontSize(fontSize > 0 ? fontSize : 12),
m_color(color),
m_textRenderer(TextRenderer::getInstance()),
// No m_cacheTexture or m_texture initialization here
m_width(0), m_height(0), m_x(0), m_y(0)
{
if (!m_textRenderer.isInitialized()) { /*...*/ }
else {
SDL_Point size = m_textRenderer.getTextSize(m_text, m_fontSize);
m_width = size.x;
m_height = size.y;
}
SDL_Point pos = XenUI::CalculateFinalPosition(m_posParams, m_width, m_height);
m_x = pos.x;
m_y = pos.y;

// No call to generateCache() needed here
}


// these are no longer needed. 

// Label::~Label() {
//     if (m_cachedTexture) {
//         SDL_DestroyTexture(m_cachedTexture);
//         m_cachedTexture = nullptr;
//     }
// }

// void Label::generateCache() {
//     int width, height;
//     SDL_Texture* tex = m_textRenderer.renderTextToTexture(m_text, m_color, width, height);
//     if (tex) {
//         if (m_cachedTexture) SDL_DestroyTexture(m_cachedTexture);
//         m_cachedTexture = tex;
//         m_cachedWidth = width;
//         m_cachedHeight = height;
//     }
// }

// CHANGE: Updated setText
// In Label.cpp
void Label::setText(const std::string& text) {
    if (m_text == text) return;
    m_text = text;
    // Recalculate size needed
    SDL_Point size = m_textRenderer.getTextSize(m_text, m_fontSize);
    m_width = size.x;
    m_height = size.y;
    // Recalculate position based on potentially new size
    recalculatePosition();
    // No texture cache update needed here
}

void Label::setPosition(int x, int y) {
    m_x = x;
    m_y = y;
}

// void Label::setScale(float scale) {
//     m_scale = scale > 0 ? scale : 1.0f;
//     if (m_cacheText) {
//         generateCache(); // re-generate with new scale
//     }
// }

void Label::setColor(const SDL_Color& color) {
    m_color = color;
    
}

// CHANGE: Updated draw method
// In Label.cpp
void Label::draw() {
    if (!m_textRenderer.isInitialized() || m_text.empty()) return;
    // This single call handles everything, including TextRenderer's internal cache
    m_textRenderer.renderText(m_text, m_x, m_y, m_color, m_fontSize);
}

void Label::recalculatePosition() {
    SDL_Point size = m_textRenderer.getTextSize(m_text, m_fontSize);
    SDL_Point pos = XenUI::CalculateFinalPosition(m_posParams, size.x, size.y);
    m_x = pos.x;
    m_y = pos.y;
}


// In the file where XenUI::Label is implemented (e.g., Label.cpp or ImmediateUI.cpp)

// In the file where XenUI::Label is implemented (e.g., Label.cpp or ImmediateUI.cpp)

namespace XenUI {

    // === MODIFIED IMMEDIATE MODE LABEL ===
    void Label(const std::string& text,
               const XenUI::PositionParams& posParams,
               int fontSize, // <<< Changed from scale
               const SDL_Color& color) // <<< Removed enableCache flag
    {
        auto& textRenderer = TextRenderer::getInstance();

        if (!textRenderer.isInitialized() || text.empty() || fontSize <= 0) {
            // std::cerr << "TextRenderer not initialized or text/fontSize invalid for immediate Label!\n";
            return;
        }

        int w = 0, h = 0; // Use int for width/height
        SDL_Texture* tex = nullptr;

        // Always use renderTextToTexture, which handles caching internally now
        tex = textRenderer.renderTextToTexture(text, color, fontSize, w, h);

        if (tex) {
            // Calculate final position based on PositionParams and ACTUAL texture size (w, h)
            SDL_Point pos = XenUI::CalculateFinalPosition(posParams, w, h);

            // Render using calculated position and texture size (NO scaling)
            SDL_Rect dst = {pos.x, pos.y, w, h}; // Use actual width and height
            SDL_RenderCopy(textRenderer.getRenderer(), tex, nullptr, &dst);

            // *** DO NOT DestroyTexture here! ***
            // The texture belongs to the TextRenderer's cache.
        }
        // else { // Optional: Error logging if texture creation failed
        //    std::cerr << "Immediate Label: Failed to get texture for '" << text << "'\n";
        // }
    }

} // namespace XenUI



