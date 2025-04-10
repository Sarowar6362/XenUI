#include "Button.h"
#include "../utils/Anchor.h"
#include "../utils/WindowUtil.h"
#include "../graphics/sdl2/TextRenderer.h"
#include <string>

// Button.cpp (or wherever Button implementation is)
 // Assuming Button.h is in the correct relative path
 // Need this for GetWindowSize if ResolveAnchorPosition uses it implicitly

#include <iostream>
#include <utility> // For std::move

// Make sure TextRenderer instance is accessible
extern TextRenderer& textRenderer;

    // --- MODIFIED Button Constructor ---
    Button::Button(const std::string& text, 
        const XenUI::PositionParams& posParams,
        ButtonStyle style, 
        std::function<void()> onClick)
    : m_text(text),
    m_posParams(posParams), // Store the passed PositionParams
    m_style(style),
    m_onClick(std::move(onClick)),
    m_hovered(false),
    m_posX(0), // Initialize position/size
    m_posY(0),
    m_width(0),
    m_height(0)
{
    // Ensure TextRenderer is initialized before using it
    if (!textRenderer.isInitialized()) {
    std::cerr << "ERROR: TextRenderer not initialized when creating Button '" << text << "'!" << std::endl;
    m_width = 50;
    m_height = 20;
    } else {
    // 1. Calculate the size needed for the text
    int textW = 0, textH = 0;
    textRenderer.measureText(m_text, textW, textH);

    // 2. Calculate the button's full width and height including padding
    m_width = textW + 2 * m_style.paddingX;
    m_height = textH + 2 * m_style.paddingY;
    }

    // 3. Calculate the initial screen position using the central function
    SDL_Point finalPos = XenUI::CalculateFinalPosition(m_posParams, m_width, m_height);
    m_posX = finalPos.x;
    m_posY = finalPos.y;

    // Debug output (optional)
    // std::cout << "Retained Button '" << m_text << "' created. Mode: "
    // << (m_posParams.mode == XenUI::PositionMode::ABSOLUTE ? "Abs" : "Anch")
    // << ", Size: (" << m_width << ", " << m_height
    // << "), Final Pos: (" << m_posX << ", " << m_posY << ")" << std::endl;
}

// --- draw() method (Should be OK - uses m_posX, m_posY, m_width, m_height) ---
void Button::draw(SDL_Renderer* renderer) {
    // Define the rectangle using member variables
    SDL_Rect rect = {m_posX, m_posY, m_width, m_height};

    // *** ADD THIS MISSING SECTION BACK ***

    // Draw button background using the button's stored style
    if (m_style.drawBackground) {
        SDL_SetRenderDrawColor(renderer, m_style.bgColor.r, m_style.bgColor.g, m_style.bgColor.b, m_style.bgColor.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Draw button border using the button's stored style
    if (m_style.drawBorder) {
        // Using white border for consistency with immediate mode example
        // You could potentially add borderColor to ButtonStyle later if needed
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }

    // *** END OF MISSING SECTION ***

    // Draw Centered Text (This part was already correct)
    if (!m_text.empty() && textRenderer.isInitialized()) {
        int textW, textH;
        textRenderer.measureText(m_text, textW, textH);
        int textX = m_posX + (m_width - textW) / 2;
        int textY = m_posY + (m_height - textH) / 2;
        textRenderer.renderText(m_text, textX, textY, m_style.textColor);
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
                const PositionParams& posParams, // Use PositionParams
                ButtonStyle style) {

        SDL_Renderer* renderer = textRenderer.getRenderer();
        if (!renderer || !textRenderer.isInitialized()) {
             // std::cerr << "Error: Renderer or TextRenderer not ready for immediate Button '" << id << "'\n";
             return false;
        }

        // === Calculate text dimensions ===
        int textW, textH;
        textRenderer.measureText(text, textW, textH);

        // === Auto-size button ===
        int width = textW + 2 * style.paddingX;
        int height = textH + 2 * style.paddingY;

        // === Calculate Position using Central Function ===
        // *** Use the central calculation function ***
        SDL_Point finalPos = CalculateFinalPosition(posParams, width, height);
        int finalX = finalPos.x;
        int finalY = finalPos.y;
        // ---

        // === Drawing Rect ===
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
        textRenderer.renderText(text, textX, textY, style.textColor);
        // ...

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



