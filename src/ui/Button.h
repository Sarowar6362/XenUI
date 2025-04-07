#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <string>
#include <functional>  // ✅ Added to fix std::function error
#include "../graphics/sdl2/TextRenderer.h"


// Define ButtonStyle structure
struct ButtonStyle {
    SDL_Color bgColor;
    SDL_Color textColor;
    bool drawBackground = true;
    bool drawBorder = true;
    int paddingX = 10;
    int paddingY = 5;
};

class Button {
public:
    Button(const std::string& text, int x, int y, int width, int height,
           ButtonStyle style, std::function<void()> onClick);

    void draw(SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& event);

private:
    std::string m_text;
    int m_x, m_y, m_width, m_height;
    ButtonStyle m_style;
    std::function<void()> m_onClick;
    bool m_hovered;
};



// === IMMEDIATE MODE BUTTON API ===
namespace XenUI {
    // Returns true if the button is clicked
    
        
        
        bool Button(const std::string& id, const std::string& text, int x, int y,
            ButtonStyle style = {});
}


#endif
