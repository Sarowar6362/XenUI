#include "Button.h"
#include <iostream>

// Global text renderer instance
extern TextRenderer& textRenderer;

Button::Button(const std::string& text, int x, int y, int width, int height,
               SDL_Color bgColor, SDL_Color textColor, std::function<void()> onClick)
    : m_text(text), m_x(x), m_y(y), m_width(width), m_height(height),
      m_bgColor(bgColor), m_textColor(textColor), m_onClick(onClick), m_hovered(false) {}

void Button::draw(SDL_Renderer* renderer) {  // ✅ Pass SDL_Renderer explicitly
    // Draw button background
    SDL_SetRenderDrawColor(renderer, m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
    SDL_Rect rect = {m_x, m_y, m_width, m_height};
    SDL_RenderFillRect(renderer, &rect);

    // Draw button border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // Draw button text (centered inside button)
    int textX = m_x + (m_width / 4);
    int textY = m_y + (m_height / 4);
    textRenderer.renderText(m_text, textX, textY, m_textColor);
}

void Button::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= m_x && mouseX <= (m_x + m_width) &&
            mouseY >= m_y && mouseY <= (m_y + m_height)) {
            std::cout << "Button Clicked: " << m_text << std::endl;
            if (m_onClick) {
                m_onClick();  // ✅ Execute the button's action
            }
        }
    }
}
