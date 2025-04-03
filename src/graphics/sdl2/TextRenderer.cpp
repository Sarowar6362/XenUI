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
    if (!m_initialized) {
        std::cerr << "TextRenderer is not initialized!" << std::endl;
        return;
    }

    // Render the text to a surface using anti-aliasing (Blended)
    SDL_Surface* textSurface = TTF_RenderText_Blended(m_font, text.c_str(), color);
    if (textSurface == nullptr) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Create texture from the surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    if (textTexture == nullptr) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }

    // Define where to render the text
    SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};
    
    // Copy the texture to the renderer
    SDL_RenderCopy(m_renderer, textTexture, nullptr, &renderQuad);

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

