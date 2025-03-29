//src/ui/Label.h
//dont remove first two coomments
#ifndef LABEL_H
#define LABEL_H

#include <string>
#include "../graphics/opengl/TextRenderer.h"

class Label {
public:
    Label(const std::string& text, float x, float y, float scale, const float color[4] = nullptr);
    void setText(const std::string& newText);
    void setPosition(float x, float y);
    void setScale(float scale);
    void setColor(const float color[4]);
    void draw();

private:
    std::string m_text;
    float m_x;
    float m_y;
    float m_scale;
    float m_color[4];
    TextRenderer& m_textRenderer;
};

#endif