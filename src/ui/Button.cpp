#include "Button.h"
#include <iostream>

// Global text renderer instance
extern TextRenderer& textRenderer;

Button::Button(const std::string& text, int x, int y, int width, int height,
               ButtonStyle style, std::function<void()> onClick) 
    : m_text(text), m_x(x), m_y(y), m_style(style), m_onClick(onClick), m_hovered(false) {

    // === Calculate text dimensions ===
    int textW, textH;
    textRenderer.measureText(m_text, textW, textH);

    // Auto-size button based on text + padding
    m_width = textW + 2 * m_style.paddingX;
    m_height = std::max(height, textH + 2 * m_style.paddingY);  // Ensure button height is large enough for text
}

void Button::draw(SDL_Renderer* renderer) {
    SDL_Rect rect = {m_x, m_y, m_width, m_height};

    // Draw button background
    if (m_style.drawBackground) {
        SDL_SetRenderDrawColor(renderer, m_style.bgColor.r, m_style.bgColor.g, m_style.bgColor.b, m_style.bgColor.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Draw button border
    if (m_style.drawBorder) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }

    // Draw Centered Text
    int textW, textH;
    textRenderer.measureText(m_text, textW, textH);
    int textX = m_x + (m_width - textW) / 2;
    int textY = m_y + (m_height - textH) / 2;
    textRenderer.renderText(m_text, textX, textY, m_style.textColor);
}

void Button::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= m_x && mouseX <= (m_x + m_width) &&
            mouseY >= m_y && mouseY <= (m_y + m_height)) {
            std::cout << "Button Clicked: " << m_text << std::endl;
            if (m_onClick) {
                m_onClick();  // Execute the button's action
            }
        }
    }
}





// === IMMEDIATE MODE BUTTON ===
#include <unordered_map>

namespace XenUI {
    struct ButtonState {
        bool isPressed = false;
        bool wasInside = false;
    };

    bool Button(const std::string& id, const std::string& text, int x, int y,
                ButtonStyle style) {

        SDL_Renderer* renderer = textRenderer.getRenderer();
        static std::unordered_map<std::string, ButtonState> buttonStates;

        // === Calculate text dimensions ===
        int textW, textH;
        textRenderer.measureText(text, textW, textH);

        // Auto-size button based on text + padding
        int width = textW + 2 * style.paddingX;
        int height = textH + 2 * style.paddingY;

        SDL_Rect rect = {x, y, width, height};

        // === Draw Background ===
        if (style.drawBackground) {
            SDL_SetRenderDrawColor(renderer, style.bgColor.r, style.bgColor.g, style.bgColor.b, style.bgColor.a);
            SDL_RenderFillRect(renderer, &rect);
        }

        if (style.drawBorder) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }

        // === Draw Centered Text ===
        int textX = x + (width - textW) / 2;
        int textY = y + (height - textH) / 2;
        textRenderer.renderText(text, textX, textY, style.textColor);

        // === Handle Input ===
        ButtonState& state = buttonStates[id];
        int mouseX, mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        bool inside = (mouseX >= x && mouseX <= (x + width) &&
                       mouseY >= y && mouseY <= (y + height));

        if ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && inside) {
            state.isPressed = true;
            state.wasInside = true;
        }

        if (!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && state.isPressed && state.wasInside) {
            state.isPressed = false;
            state.wasInside = false;
            return true;
        }

        if (!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
            state.isPressed = false;
            state.wasInside = false;
        }

        return false;
    }
}



