#include "Label.h"
#include "../graphics/sdl2/TextRenderer.h"
#include <iostream>



Label::Label(const std::string& text, int x, int y, float scale, const SDL_Color& color, bool cacheText)
    : m_text(text), m_x(x), m_y(y), m_scale(scale),
      m_color(color), m_textRenderer(TextRenderer::getInstance()), m_cacheText(cacheText) {

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




// === IMMEDIATE MODE LABEL ===
namespace XenUI {

    void Label(const std::string& text, int x, int y, float scale, const SDL_Color& color, bool enableCache) {
        auto& textRenderer = TextRenderer::getInstance();
    
        if (!textRenderer.isInitialized()) {
            std::cerr << "TextRenderer not initialized!\n";
            return;
        }
    
        if (enableCache) {
            int w, h;
            SDL_Texture* tex = textRenderer.renderTextToTexture(text, color, w, h);
            if (tex) {
                SDL_Rect dst = {x, y, int(w * scale), int(h * scale)};
                SDL_RenderCopy(textRenderer.getRenderer(), tex, nullptr, &dst);
            }
        } else {
            textRenderer.renderText(text, x, y, color);
        }


        // Inside XenUI::Label(...)
        if (enableCache) {
            int w, h;
            SDL_Texture* tex = textRenderer.renderTextToTexture(text, color, w, h);
            if (tex) {
                SDL_Rect dst = {x, y, int(w * scale), int(h * scale)};
                SDL_RenderCopy(textRenderer.getRenderer(), tex, nullptr, &dst);
            }
        } else {
            int w, h;
            SDL_Texture* tex = textRenderer.renderTextImmediateToTexture(text, color, w, h);
            if (tex) {
                SDL_Rect dst = {x, y, int(w * scale), int(h * scale)};
                SDL_RenderCopy(textRenderer.getRenderer(), tex, nullptr, &dst);
                SDL_DestroyTexture(tex); // clean up right after use
            }
        }

    }
    

    
    }
    