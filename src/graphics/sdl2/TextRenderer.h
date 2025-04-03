//src/graphics/sdl2/TextRenderer.h
//dont remove first two comments
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <fstream>


class TextRenderer {
public:
    static TextRenderer& getInstance();
    
    void init(SDL_Renderer* renderer, const std::string& fontPath, int fontSize);
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    bool isInitialized() const { return m_initialized; } // Checks if the renderer is initialized

private:
    TextRenderer();
    ~TextRenderer();
    
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;
    bool m_initialized;
};

#endif
