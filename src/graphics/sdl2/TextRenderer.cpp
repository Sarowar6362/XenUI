//XenonUI/src/graphics/sdl2/TextRenderer.cpp
#include "TextRenderer.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <sstream>

TextRenderer::TextRenderer() : m_renderer(nullptr), m_initialized(false) {}

TextRenderer::~TextRenderer() {
    clearCache();
    
    TTF_Quit();  // Quit SDL_ttf when done
}

TextRenderer& TextRenderer::getInstance() {
    static TextRenderer instance;
    return instance;
}




void TextRenderer::init(SDL_Renderer* renderer, const std::string& fontPath) {
        if (m_initialized) return;
        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
            m_initialized = false;
            return;
        }
        m_renderer = renderer;
        m_fontPath = fontPath; // Store path
        m_initialized = true;  // TTF_Init is in main
        std::cout << "TextRenderer initialized. Font path: " << m_fontPath << std::endl;
    }
    

// ADD: getFont implementation
TTF_Font* TextRenderer::getFont(int fontSize) {
    if (!m_initialized || m_fontPath.empty() || fontSize <= 0) return nullptr;
    auto it = m_fontsBySize.find(fontSize);
    if (it != m_fontsBySize.end()) {
        return it->second; // Found in map
    }
    // Not found, load it
    TTF_Font* font = TTF_OpenFont(m_fontPath.c_str(), fontSize);
    if (!font) {
        std::cerr << "getFont: Failed to open font '" << m_fontPath << "' size " << fontSize << ". Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    m_fontsBySize[fontSize] = font; // Add to map
    return font;
}


// CHANGE: Updated renderText
void TextRenderer::renderText(const std::string& text, int x, int y, SDL_Color color, int fontSize) {
    if (!m_initialized) return;
    int texW = 0, texH = 0;
    // Get/create texture using the internal caching method (which now uses size)
    SDL_Texture* texture = renderTextToTexture(text, color, fontSize, texW, texH);
    if (!texture) return;
    SDL_Rect dstRect = { x, y, texW, texH };
    SDL_RenderCopy(m_renderer, texture, nullptr, &dstRect);
}

// CHANGE: Updated renderTextToTexture (used by renderText)
SDL_Texture* TextRenderer::renderTextToTexture(const std::string& text, SDL_Color color, int fontSize, int& outW, int& outH) {
    if (!m_initialized || text.empty()) {/*... set 0 return null ...*/}

    std::string key = createCacheKey(text, fontSize); // Use new key function
    auto cacheIt = m_textureCache.find(key);
    if (cacheIt != m_textureCache.end()) {
        outW = cacheIt->second.width; outH = cacheIt->second.height;
        return cacheIt->second.texture; // Return cached
    }

   TTF_Font* font = getFont(fontSize); // Use getFont
   if (!font) return nullptr;

   SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
   // ... (Surface check) ...
   SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
   // ... (Texture check) ...
   if (texture) {
       outW = surface->w; outH = surface->h;
       m_textureCache[key] = {texture, outW, outH}; // Add to cache with new key
   }
   SDL_FreeSurface(surface);
   return texture;
}


SDL_Renderer* TextRenderer::getRenderer() const {
    return m_renderer;
}


// void TextRenderer::clearCache() {
//     for (auto& pair : textCache) {
//         if (pair.second) {
//             SDL_DestroyTexture(pair.second);
//         }
//     }
//     textCache.clear();
// }

// CHANGE: Updated clearCache
void TextRenderer::clearCache() {
    // Clear textures
    for (auto const& [key, val] : m_textureCache) {
        if (val.texture) SDL_DestroyTexture(val.texture);
    }
    m_textureCache.clear();
    // Clear fonts
    for (auto const& [size, font] : m_fontsBySize) {
        if (font) TTF_CloseFont(font);
    }
    m_fontsBySize.clear();
    std::cout << "Cleared TextRenderer cache (textures and fonts)." << std::endl;
}


// ADD: createCacheKey implementation
std::string TextRenderer::createCacheKey(const std::string& text, int fontSize) {
    std::stringstream ss;
    ss << text << '|' << fontSize; // Include size in key
    return ss.str();
}


// In TextRenderer.cpp

// NEW: For immediate-mode rendering (no caching, caller destroys texture)
SDL_Texture* TextRenderer::renderTextImmediateToTexture(const std::string& text, SDL_Color color, int fontSize, int& outW, int& outH) {
    // Initialize output parameters
    outW = 0;
    outH = 0;

    if (!m_initialized || text.empty() || fontSize <= 0) {
        std::cerr << "Error: TextRenderer not ready or invalid input for immediate render." << std::endl;
        return nullptr;
    }

    // *** Use getFont() to get the correct font size ***
    TTF_Font* font = getFont(fontSize);
    if (!font) {
        // getFont already prints an error if it fails
        return nullptr;
    }

    // *** Render using the obtained font ***
    // Using Blended for better quality with alpha, consider UTF8 for broader text support
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Error creating immediate surface: " << TTF_GetError() << std::endl;
        // Do NOT close the font here, it's managed by the m_fontsBySize map
        return nullptr;
    }

    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (texture) {
        // Get dimensions if texture creation succeeded
        outW = surface->w;
        outH = surface->h;
    } else {
        std::cerr << "Error creating immediate texture: " << SDL_GetError() << std::endl;
        // Texture is nullptr, outW/outH remain 0
    }

    // Free the temporary surface, we don't need it anymore
    SDL_FreeSurface(surface);

    // Return the newly created texture.
    // IMPORTANT: The caller of this function is responsible for calling
    // SDL_DestroyTexture() on the returned texture when done.
    return texture;
}

// CHANGE: Updated measureText/getTextSize
void TextRenderer::measureText(const std::string& text, int fontSize, int& w, int& h) {
    if (!m_initialized || text.empty()) {/*... set 0 ...*/ return; }
    TTF_Font* font = getFont(fontSize); // Use getFont
    if (!font) {/*... set 0 ...*/ return; }
    if (TTF_SizeUTF8(font, text.c_str(), &w, &h) != 0) { /*...*/ }
    // NO TTF_CloseFont here!
}

SDL_Point TextRenderer::getTextSize(const std::string& text, int fontSize) {
    SDL_Point size = {0, 0};
    measureText(text, fontSize, size.x, size.y);
    return size;
}

