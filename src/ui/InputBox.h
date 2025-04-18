// include/XenUI/InputBox.h (or ui/InputBox.h if installed there)
#ifndef XENUI_INPUTBOX_H
#define XENUI_INPUTBOX_H

#include "../utils/Position.h" // Assumes Position.h is public
//#include "../utils/ButtonStyle.h" // Reuse or adapt ButtonStyle if suitable
#include "../graphics/sdl2/TextRenderer.h" // Forward declare if possible? No, need getInstance type maybe. Include for now.
#include <string>
#include <functional>
#include <SDL2/SDL.h>

namespace XenUI {

// Example: Define InputBoxStyle, maybe inheriting or containing ButtonStyle parts
struct InputBoxStyle {
    SDL_Color bgColor = { 225, 40, 40, 255 };
    SDL_Color textColor = { 230, 230, 230, 255 };
    SDL_Color borderColor = { 80, 200, 80, 255 };
    SDL_Color cursorColor = { 240, 240, 240, 255 };
    SDL_Color selectionBgColor = { 70, 100, 130, 150 }; // Semi-transparent blueish
    SDL_Color focusedBorderColor = { 100, 150, 255, 255 }; // Border when focused
    int paddingX = 5;
    int paddingY = 3;
    bool drawBackground = true;
    bool drawBorder = true;
};

const int DEFAULT_INPUT_FONT_SIZE = 16;

class InputBox {
public:
    InputBox(const PositionParams& posParams,
             const std::string& initialText = "",
             int width = 200, // Initial width hint
             int fontSize = DEFAULT_INPUT_FONT_SIZE,
             InputBoxStyle style = {},
             bool isPassword = false);

    // Core methods called by application or framework
    void handleEvent(const SDL_Event& event);
    void update(float deltaTime); // Needed for cursor blinking
   // void draw(); // Needs access to renderer internally now
    void recalculatePosition(); // Uses m_posParams
    void draw(SDL_Renderer* renderer); // <<< Add renderer parameter

    // Getters/Setters
    std::string getText() const;
    void setText(const std::string& text);
    void focus();   // Programmatically give focus
    void unfocus(); // Programmatically remove focus

    // Callbacks (optional)
    std::function<void(const std::string&)> onTextChanged;
    std::function<void(const std::string&)> onEnterPressed; // Passes final text

private:
    // --- Internal State ---
    PositionParams m_posParams;
    std::string m_text;         // The actual text content
    std::string m_displayText;  // Text actually drawn (might be '*' for password)
    InputBoxStyle m_style;
    int m_fontSize;
    bool m_isPassword;

    int m_posX = 0, m_posY = 0; // Calculated top-left
    int m_width = 0;            // Current actual width (might differ from initial hint)
    int m_height = 0;           // Calculated height based on font & padding

    bool m_hasFocus = false;
    int m_cursorPos = 0;        // Character index in m_text (0 to m_text.length())
    int m_selectionStart = -1;  // Start index for selection, or -1 if none

    // --- Rendering & Scrolling ---
    int m_scrollX = 0;          // Horizontal scroll offset in pixels
    bool m_isCursorVisible = true; // Toggled for blinking
    float m_cursorBlinkTimer = 0.0f;
    static constexpr float CURSOR_BLINK_RATE = 0.53f; // Blink interval in seconds

    TextRenderer& m_textRenderer; // Reference to the singleton

    // --- Internal Helpers ---
    void updateDisplayText();        // Updates m_displayText (e.g., for password masking)
    void clampCursorAndScroll();     // Ensures cursor is valid and scrolls view if needed
    int getIndexFromXCoord(int mouseX); // Maps screen X within box to text index
    int getTextXPosition(int charIndex); // Gets screen X relative to box start for a char index
    void deleteSelection();          // Deletes currently selected text
    //void handleKeyDown(const SDL_Keycode& key, const SDL_Keymod& mod); // Handles key presses
    void handleKeyDown(SDL_Keycode key, SDL_Keymod mod);
    void handleTextInput(const char* inputText); // Handles text input events
    void handleMouseInput(const SDL_Event& event); // Handles mouse down/up/motion
    void calculateHeight();          // Calculates m_height based on font/padding
};

} // namespace XenUI
#endif // XENUI_INPUTBOX_H