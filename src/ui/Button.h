#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <string>
#include <functional>  // ✅ Added to fix std::function error
#include "../graphics/sdl2/TextRenderer.h"

class Button {
private:
    std::string m_text;
    int m_x, m_y, m_width, m_height;
    SDL_Color m_bgColor, m_textColor;
    bool m_hovered;
    std::function<void()> m_onClick;  // ✅ Moved inside private section

public:
    Button(const std::string& text, int x, int y, int width, int height,
           SDL_Color bgColor, SDL_Color textColor, std::function<void()> onClick);

    void draw(SDL_Renderer* renderer);  // ✅ Fixed: Now accepts renderer
    void handleEvent(const SDL_Event& event);
};

#endif
