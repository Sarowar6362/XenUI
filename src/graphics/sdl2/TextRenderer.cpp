#include "TextRenderer.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>

TextRenderer::TextRenderer() : m_renderer(nullptr), m_font(nullptr), m_initialized(false) {}

TextRenderer::~TextRenderer() {
    if (m_font) {
        TTF_CloseFont(m_font);
    }
    TTF_Quit();  // Quit SDL_ttf when done
}

TextRenderer& TextRenderer::getInstance() {
    static TextRenderer instance;
    return instance;
}

void TextRenderer::init(SDL_Renderer* renderer, const std::string& fontPath, int fontSize) {
    m_renderer = renderer;
    
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        m_initialized = false;
        return;
    }

    m_font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (m_font == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        m_initialized = false;
        return;
    }

    m_initialized = true;
}

void TextRenderer::renderText(const std::string& text, int x, int y, SDL_Color color) {
    std::string key = text + std::to_string(color.r) + std::to_string(color.g) +
                      std::to_string(color.b) + std::to_string(color.a);

    // Check if the texture already exists in the cache
    if (textCache.find(key) == textCache.end()) {
        SDL_Surface* surface = TTF_RenderText_Blended(m_font, text.c_str(), color);
        if (!surface) {
            std::cerr << "Error creating surface: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
            return;
        }

        // Store the generated texture in the cache
        textCache[key] = texture;
    }

    // Get cached texture
    SDL_Texture* texture = textCache[key];

    // Get texture size
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

    SDL_Rect destRect = {x, y, w, h};
    SDL_RenderCopy(m_renderer, texture, nullptr, &destRect);
}

void TextRenderer::clearCache() {
    for (auto& pair : textCache) {
        SDL_DestroyTexture(pair.second);
    }
    textCache.clear();
}




