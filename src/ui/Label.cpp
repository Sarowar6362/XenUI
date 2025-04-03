#include "Label.h"
#include <cstring>  // For memcpy

Label::Label(const std::string& text, int x, int y, float scale, const SDL_Color& color)
    : m_text(text), m_x(x), m_y(y), m_scale(scale),
      m_textRenderer(TextRenderer::getInstance()), m_color(color) {}

void Label::setText(const std::string& newText) {
    m_text = newText;
}

void Label::setPosition(int x, int y) {
    m_x = x;
    m_y = y;
}

void Label::setScale(float scale) {
    m_scale = scale > 0 ? scale : 1.0f;  // Ensure positive scale
}

void Label::setColor(const SDL_Color& color) {
    m_color = color;
}

void Label::draw() {
    if (!m_text.empty()) {
        m_textRenderer.renderText(m_text, m_x, m_y, m_color); // Use updated m_x, m_y
    }
}
