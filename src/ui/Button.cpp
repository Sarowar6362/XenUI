#include "Button.h"
#include "../utils/Anchor.h"
#include "../utils/WindowUtil.h"
#include "../utils/Position.h"
#include "../graphics/sdl2/TextRenderer.h"
#include <string>

// Button.cpp (or wherever Button implementation is)
 // Assuming Button.h is in the correct relative path
 // Need this for GetWindowSize if ResolveAnchorPosition uses it implicitly

#include <iostream>
#include <utility> // For std::move

// Make sure TextRenderer instance is accessible
extern TextRenderer& textRenderer; // Ensure this is accessible

// --- MODIFIED Retained Button Constructor ---
Button::Button(const std::string& text,
               const XenUI::PositionParams& posParams,
               ButtonStyle style,
               std::function<void()> onClick,
               int fontSize) // *** Accept fontSize ***
    : m_text(text),
      m_posParams(posParams),
      m_style(style),
      m_onClick(std::move(onClick)),
      m_fontSize(fontSize > 0 ? fontSize : DEFAULT_BUTTON_FONT_SIZE), // *** Store fontSize ***
      m_hovered(false),
      m_posX(0), m_posY(0), m_width(0), m_height(0)
{
    if (!textRenderer.isInitialized()) { /* ... error handling ... */ }
    else {
        int textW = 0, textH = 0;
        // *** PASS m_fontSize to measureText ***
        textRenderer.measureText(m_text, m_fontSize, textW, textH);

        m_width = textW + 2 * m_style.paddingX;
        m_height = textH + 2 * m_style.paddingY;
    }
    SDL_Point finalPos = XenUI::CalculateFinalPosition(m_posParams, m_width, m_height);
    m_posX = finalPos.x; m_posY = finalPos.y;
    // ... debug output ...
}

// --- MODIFIED Retained Button draw() method ---
void Button::draw(SDL_Renderer* renderer) {
    SDL_Rect rect = {m_posX, m_posY, m_width, m_height};
    // ... background drawing ...
    if (m_style.drawBackground) { 
        SDL_SetRenderDrawColor(renderer, m_style.bgColor.r, m_style.bgColor.g, m_style.bgColor.b, m_style.bgColor.a);
        SDL_RenderFillRect(renderer, &rect);
     }
    // ... border drawing ...
    if (m_style.drawBorder) { 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }

    // Draw Centered Text
    if (!m_text.empty() && textRenderer.isInitialized()) {
        int textW, textH;
        // *** PASS m_fontSize to measureText ***
        textRenderer.measureText(m_text, m_fontSize, textW, textH);
        int textX = m_posX + (m_width - textW) / 2;
        int textY = m_posY + (m_height - textH) / 2;
        // *** PASS m_fontSize to renderText ***
        textRenderer.renderText(m_text, textX, textY, m_style.textColor, m_fontSize);
    }
}




// --- handleEvent() method (Should be OK - uses m_posX, m_posY, m_width, m_height) ---
void Button::handleEvent(const SDL_Event& event) {
     if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            // Check using calculated position and size
            if (mouseX >= m_posX && mouseX <= (m_posX + m_width) &&
                mouseY >= m_posY && mouseY <= (m_posY + m_height))
            {
                // std::cout << "Button Clicked: " << m_text << std::endl;
                if (m_onClick) {
                    m_onClick();
                }
            }
        }
    }
    // ... (rest of event logic) ...
}

// --- MODIFIED recalculatePosition ---
void Button::recalculatePosition() {
    // Recalculate size MAYBE needed if style/text changed, but usually just position
    // If size can change dynamically, recalculate m_width/m_height here first.

    // Resolve the final screen position again using the stored parameters
    SDL_Point finalPos = XenUI::CalculateFinalPosition(m_posParams, m_width, m_height);
    m_posX = finalPos.x;
    m_posY = finalPos.y;
    // std::cout << "Button '" << m_text << "' Repositioned: (" << m_posX << ", " << m_posY << ")" << std::endl; // Debug
}



// === IMMEDIATE MODE BUTTON ===
#include <unordered_map>

// In Button.cpp or wherever the immediate mode function implementation resides
// ... (includes needed: Button.h, Anchor.h, Position.h, WindowUtil.h, TextRenderer.h, etc.)

// Ensure textRenderer instance is accessible


namespace XenUI {

    // State map needs to be accessible
    namespace Detail {
        struct ButtonState { bool isPressed = false; bool wasInside = false; };
        static std::unordered_map<std::string, ButtonState> buttonStates;
    }

     // --- MODIFIED Immediate Mode Button Implementation ---
     bool Button(const std::string& id, const std::string& text,
        const PositionParams& posParams,
        ButtonStyle style,
        int fontSize) // *** Accept fontSize ***
{

        SDL_Renderer* renderer = textRenderer.getRenderer();
        if (!renderer || !textRenderer.isInitialized()) {
             // std::cerr << "Error: Renderer or TextRenderer not ready for immediate Button '" << id << "'\n";
             return false;
        }

        int textW, textH;
        // *** PASS fontSize to measureText ***
        textRenderer.measureText(text, fontSize, textW, textH);

        int width = textW + 2 * style.paddingX;
        int height = textH + 2 * style.paddingY;

        SDL_Point finalPos = CalculateFinalPosition(posParams, width, height);
        int finalX = finalPos.x; int finalY = finalPos.y;
        SDL_Rect rect = {finalX, finalY, width, height};

        // === Draw Background and Border (using finalX, finalY, width, height) ===
         if (style.drawBackground) {
            SDL_SetRenderDrawColor(renderer, style.bgColor.r, style.bgColor.g, style.bgColor.b, style.bgColor.a);
            SDL_RenderFillRect(renderer, &rect);
        }
        if (style.drawBorder) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Example: White border
            SDL_RenderDrawRect(renderer, &rect);
        }
        // ...

        // === Draw Centered Text (using finalX, finalY, width, height) ===
        int textX = finalX + (width - textW) / 2;
        int textY = finalY + (height - textH) / 2;
        
        // ...
        textRenderer.renderText(text, textX, textY, style.textColor, fontSize);
        // === Handle Input (using finalX, finalY, width, height) ===
        Detail::ButtonState& state = Detail::buttonStates[id];
        int mouseX, mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        bool inside = (mouseX >= finalX && mouseX <= (finalX + width) &&
                       mouseY >= finalY && mouseY <= (finalY + height));

        bool clicked = false;
        // ... (Rest of click detection logic remains the same, using 'inside') ...
        if ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && inside && !state.isPressed) {
             state.isPressed = true;
             state.wasInside = true;
        } else if (!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && state.isPressed && state.wasInside && inside) {
             clicked = true;
             state.isPressed = false;
             state.wasInside = false;
        } else if (!(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
             state.isPressed = false;
             state.wasInside = false;
        } else if (state.isPressed && !inside) {
             // state.wasInside = false; // Optional: invalidate click if mouse moves out
        }

        return clicked;
    }

} // namespace XenUI



