#include "Label.h"
#include "../graphics/sdl2/TextRenderer.h"
#include "../utils/Anchor.h"
#include "../utils/WindowUtil.h"

#include <iostream>

Label::Label(const std::string& text,
    const XenUI::PositionParams& posParams,
    float scale,
    const SDL_Color& color,
    bool cacheText)
    : 
    m_text(text), 
    m_posParams(posParams), 
    m_scale(scale > 0 ? scale : 1.0f), 
    m_color(color), 
    m_textRenderer(TextRenderer::getInstance()), 
    m_cacheText(cacheText) 
{
    SDL_Point size = m_textRenderer.getTextSize(m_text, m_scale);
    SDL_Point pos = XenUI::CalculateFinalPosition(m_posParams, size.x, size.y);

    m_x = pos.x;
    m_y = pos.y;

    if (m_cacheText && m_textRenderer.isInitialized()) {
        generateCache();
    }
}

Label::~Label() {
    if (m_cachedTexture) {
        SDL_DestroyTexture(m_cachedTexture);
        m_cachedTexture = nullptr;
    }
}

void Label::generateCache() {
    int width, height;
    SDL_Texture* tex = m_textRenderer.renderTextToTexture(m_text, m_color, width, height);
    if (tex) {
        if (m_cachedTexture) SDL_DestroyTexture(m_cachedTexture);
        m_cachedTexture = tex;
        m_cachedWidth = width;
        m_cachedHeight = height;
    }
}

void Label::setText(const std::string& newText) {
    m_text = newText;
    if (m_cacheText) {
        generateCache(); // regenerate texture if caching is on
    }
}

void Label::setPosition(int x, int y) {
    m_x = x;
    m_y = y;
}

void Label::setScale(float scale) {
    m_scale = scale > 0 ? scale : 1.0f;
    if (m_cacheText) {
        generateCache(); // re-generate with new scale
    }
}

void Label::setColor(const SDL_Color& color) {
    m_color = color;
    if (m_cacheText) {
        generateCache(); // update cached texture color
    }
}

void Label::draw() {
    if (!m_text.empty()) {
        if (m_cacheText && m_cachedTexture) {
            SDL_Rect dst = {m_x, m_y, int(m_cachedWidth * m_scale), int(m_cachedHeight * m_scale)};
            SDL_RenderCopy(m_textRenderer.getRenderer(), m_cachedTexture, nullptr, &dst);
        } else {
            m_textRenderer.renderText(m_text, m_x, m_y, m_color);
        }
    }
}

void Label::recalculatePosition() {
    SDL_Point size = m_textRenderer.getTextSize(m_text, m_scale);
    SDL_Point pos = XenUI::CalculateFinalPosition(m_posParams, size.x, size.y);
    m_x = pos.x;
    m_y = pos.y;
}



// === IMMEDIATE MODE LABEL ===
namespace XenUI {

    void Label(const std::string& text, const XenUI::PositionParams& posParams, float scale, const SDL_Color& color, bool enableCache) {
        auto& textRenderer = TextRenderer::getInstance();

        if (!textRenderer.isInitialized()) {
            std::cerr << "TextRenderer not initialized!\n";
            return;
        }

        int w, h;
        SDL_Texture* tex = nullptr;

        // Render the text to texture (cached or immediate)
        if (enableCache) {
            tex = textRenderer.renderTextToTexture(text, color, w, h);
        } else {
            tex = textRenderer.renderTextImmediateToTexture(text, color, w, h);
        }

        if (tex) {
            // Calculate final position based on PositionParams
            SDL_Point pos = XenUI::CalculateFinalPosition(posParams, w, h);

            // Apply the calculated position and scale
            SDL_Rect dst = {pos.x, pos.y, int(w * scale), int(h * scale)};
            SDL_RenderCopy(textRenderer.getRenderer(), tex, nullptr, &dst);

            if (!enableCache) {
                SDL_DestroyTexture(tex); // Clean up if not cached
            }
        }
    }

}

