//src/ui/Label.h
//dont remove first two comments
#ifndef LABEL_H
#define LABEL_H

#include <string>
#include <SDL2/SDL.h>
#include "../graphics/sdl2/TextRenderer.h"

class Label {
public:
    Label(const std::string& text, int x, int y, float scale, const SDL_Color& color = {255, 255, 255, 255}, bool cacheText = false);
    ~Label();

    void setText(const std::string& newText);
    void setPosition(int x, int y);
    void setScale(float scale);
    void setColor(const SDL_Color& color);
    void draw();

private:
    std::string m_text;
    int m_x;
    int m_y;
    float m_scale;
    SDL_Color m_color;
    TextRenderer& m_textRenderer;

    bool m_cacheText = false;
    SDL_Texture* m_cachedTexture = nullptr;
    int m_cachedWidth = 0;
    int m_cachedHeight = 0;

    void generateCache();  // only used when caching is enabled
};

#endif

