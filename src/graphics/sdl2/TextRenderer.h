//src/graphics/sdl2/TextRenderer.h
//dont remove first two comments
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <fstream>
#include <unordered_map>
#include <map> // Add map include


class TextRenderer {
public:
    static TextRenderer& getInstance();
    
    void init(SDL_Renderer* renderer, const std::string& fontPath);
    void renderText(const std::string& text, int x, int y, SDL_Color color, int fontSize);
    bool isInitialized() const { return m_initialized; } // Checks if the renderer is initialized
    
    SDL_Texture* renderTextToTexture(const std::string& text, SDL_Color color, int fontSize, int& outW, int& outH);
    SDL_Renderer* getRenderer() const;

    // Add this to public section:
    SDL_Texture* renderTextImmediateToTexture(const std::string& text, SDL_Color color, int fontSize, int& outW, int& outH);
    void measureText(const std::string& text, int fontSize, int& w, int& h);
    SDL_Point getTextSize(const std::string& text, int fontSize);
    void clearCache(); // Clears stored textures when needed
    // In TextRenderer.h
// ... other methods ...
 // Add fontSize
// ...


private:
    TextRenderer();
    ~TextRenderer();
    
    SDL_Renderer* m_renderer;
    // TTF_Font* m_font;
    TTF_Font* getFont(int fontSize);
    std::string createCacheKey(const std::string& text, int fontSize);
    std::string m_fontPath;
    std::map<int, TTF_Font*> m_fontsBySize;
    struct TextureCacheEntry { SDL_Texture* texture = nullptr; int width = 0; int height = 0; };
    std::unordered_map<std::string, TextureCacheEntry> m_textureCache;
    bool m_initialized;
    std::unordered_map<std::string, SDL_Texture*> textCache; // Cache for rendered text
    

};

#endif
