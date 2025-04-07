//src/graphics/sdl2/TextRenderer.h
//dont remove first two comments
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <fstream>
#include <unordered_map>


class TextRenderer {
public:
    static TextRenderer& getInstance();
    
    void init(SDL_Renderer* renderer, const std::string& fontPath, int fontSize);
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    bool isInitialized() const { return m_initialized; } // Checks if the renderer is initialized
    void clearCache(); // Clears stored textures when needed
    SDL_Texture* renderTextToTexture(const std::string& text, SDL_Color color, int& outW, int& outH);
    SDL_Renderer* getRenderer() const;

    // Add this to public section:
    SDL_Texture* renderTextImmediateToTexture(const std::string& text, SDL_Color color, int& outW, int& outH);
    void measureText(const std::string& text, int& w, int& h);

private:
    TextRenderer();
    ~TextRenderer();
    
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;
    bool m_initialized;
    std::unordered_map<std::string, SDL_Texture*> textCache; // Cache for rendered text
    

};

#endif
