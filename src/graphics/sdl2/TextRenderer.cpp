#include "TextRenderer.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>

TextRenderer::TextRenderer()
    : m_renderer(nullptr), m_font(nullptr), m_initialized(false) {}

TextRenderer::~TextRenderer() {
    clearCache();
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
    if (!m_font) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        m_initialized = false;
        return;
    }

    m_initialized = true;
}

void TextRenderer::renderText(const std::string& text, int x, int y, SDL_Color color) {
    int w, h;
    SDL_Texture* texture = renderTextToTexture(text, color, w, h);
    if (!texture) return;

    SDL_Rect destRect = {x, y, w, h};
    SDL_RenderCopy(m_renderer, texture, nullptr, &destRect);
}

SDL_Texture* TextRenderer::renderTextToTexture(const std::string& text, SDL_Color color, int& outW, int& outH) {
    std::string key = text + std::to_string(color.r) + std::to_string(color.g) +
                      std::to_string(color.b) + std::to_string(color.a);

    // If texture is already cached, return it
    auto it = textCache.find(key);
    if (it != textCache.end()) {
        SDL_QueryTexture(it->second, nullptr, nullptr, &outW, &outH);
        return it->second;
    }

    // Otherwise, render and cache it
    SDL_Surface* surface = TTF_RenderText_Blended(m_font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Error creating surface: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    outW = surface->w;
    outH = surface->h;
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    textCache[key] = texture;
    return texture;
}

SDL_Renderer* TextRenderer::getRenderer() const {
    return m_renderer;
}


void TextRenderer::clearCache() {
    for (auto& pair : textCache) {
        if (pair.second) {
            SDL_DestroyTexture(pair.second);
        }
    }
    textCache.clear();
}
