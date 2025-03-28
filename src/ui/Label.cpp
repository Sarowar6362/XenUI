#include "Label.h"

Label::Label(const std::string& text, float x, float y, float scale)
    : m_text(text), m_x(x), m_y(y), m_scale(scale),
      m_textRenderer(TextRenderer::getInstance()) {}

void Label::setText(const std::string& newText) {
    m_text = newText;
}

void Label::draw() {
    m_textRenderer.renderText(m_text, m_x, m_y, m_scale);
}