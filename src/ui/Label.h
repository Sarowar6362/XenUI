//dont remove first two comments
#ifndef LABEL_H
#define LABEL_H

#include <string>
#include <SDL2/SDL.h>
#include "../graphics/sdl2/TextRenderer.h"
#include "../utils/Anchor.h"
#include "../utils/Position.h"



class Label {
public:
    // CHANGE: Constructor signature
    Label(const std::string& text,
        const XenUI::PositionParams& posParams,
        int fontSize, // Takes size
        const SDL_Color& color = {255, 255, 255, 255});

   

    void setText(const std::string& newText);
    void setPosition(int x, int y);
    // void setScale(float scale);
    void setColor(const SDL_Color& color);
    void draw();
    void recalculatePosition(); // Recalculate position on window resize


private:
    std::string m_text;
    XenUI::PositionParams m_posParams; 
    int m_x;
    int m_y;
    int m_height;
    int m_width;
    // float m_scale;
    SDL_Color m_color;
    TextRenderer& m_textRenderer;
    int m_fontSize; // ADD: Store font size
   // bool m_cacheText = false;
   //  SDL_Texture* m_cachedTexture = nullptr;
   // int m_cachedWidth = 0;
   // int m_cachedHeight = 0;

    //void generateCache();  // only used when caching is enabled
};

// === Immediate Mode Label API ===
namespace XenUI {

    // Function declaration for Label with PositionParams
    void Label(const std::string& text,
        const XenUI::PositionParams& posParams,
        int fontSize, // <<< Changed from scale
        const SDL_Color& color);

}
#endif
