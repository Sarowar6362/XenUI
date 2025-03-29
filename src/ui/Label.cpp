//src/ui/Label.cpp
//dont remove first two coomments
#include "Label.h"
#include <cstring>  // For memcpy

Label::Label(const std::string& text, float x, float y, float scale, const float color[4])
    : m_text(text), m_x(x), m_y(y), m_scale(scale),
      m_textRenderer(TextRenderer::getInstance()) {
    // Default to white if no color provided
    const float defaultColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    memcpy(m_color, color ? color : defaultColor, sizeof(m_color));
}

void Label::setText(const std::string& newText) {
    m_text = newText;
}

void Label::setPosition(float x, float y) {
    m_x = x;
    m_y = y;
}

void Label::setScale(float scale) {
    m_scale = scale > 0 ? scale : 1.0f;  // Ensure positive scale
}

void Label::setColor(const float color[4]) {
    if(color) memcpy(m_color, color, sizeof(m_color));
}

void Label::draw() {
    if(!m_text.empty()) {
        m_textRenderer.renderText(m_text, m_x, m_y, m_scale, m_color);
    }
}