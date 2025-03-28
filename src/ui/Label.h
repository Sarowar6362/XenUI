#ifndef LABEL_H
#define LABEL_H

#include <string>
#include "../graphics/opengl/TextRenderer.h"

class Label {
public:
    Label(const std::string& text, float x, float y, float scale = 1.0f);
    void setText(const std::string& newText);
    void draw();

private:
    std::string m_text;
    float m_x;
    float m_y;
    float m_scale;
    TextRenderer& m_textRenderer;
};

#endif