// src/ui/InputBox.cpp
#include "InputBox.h" // Assuming path correctness
#include <SDL2/SDL_clipboard.h> // For copy/paste
#include <algorithm> // For std::min/max
#include <iostream>




namespace XenUI {

InputBox::InputBox(const PositionParams& posParams, const std::string& initialText,
                   int width, int fontSize, InputBoxStyle style, bool isPassword)
    : m_posParams(posParams),
      m_text(initialText),
      m_style(style),
      m_fontSize(fontSize > 0 ? fontSize : DEFAULT_INPUT_FONT_SIZE),
      m_isPassword(isPassword),
      m_width(width > 0 ? width : 200), // Use initial width hint
      m_textRenderer(TextRenderer::getInstance())
{
    m_cursorPos = m_text.length(); // Cursor at end initially
    calculateHeight(); // Calculate height based on font/padding
    recalculatePosition(); // Calculate initial screen position
    updateDisplayText();   // Set initial display text (handles password)
}

void InputBox::calculateHeight() {
     if (!m_textRenderer.isInitialized()) {
         m_height = (m_fontSize > 0 ? m_fontSize : 16) + 2 * m_style.paddingY; // Estimate
         return;
     }
     int w, h;
     // Measure with a typical character or placeholder like 'M' for height
     m_textRenderer.measureText("M", m_fontSize, w, h);
     m_height = h + 2 * m_style.paddingY;
}

void InputBox::recalculatePosition() {
    // Height depends on font/padding, width is currently fixed (could be dynamic later)
    SDL_Point finalPos = XenUI::CalculateFinalPosition(m_posParams, m_width, m_height);
    m_posX = finalPos.x;
    m_posY = finalPos.y;
}

void InputBox::update(float deltaTime) {
    if (m_hasFocus) {
        m_cursorBlinkTimer += deltaTime;
        if (m_cursorBlinkTimer >= CURSOR_BLINK_RATE) {
            m_isCursorVisible = !m_isCursorVisible;
            m_cursorBlinkTimer -= CURSOR_BLINK_RATE; // Reset timer more accurately
        }
    }
}


void InputBox::draw(SDL_Renderer* renderer) {
    //std::cout << "InputBox Draw: Pos(" << m_posX << "," << m_posY << ") Size(" << m_width << "," << m_height << ")\n";
    if (!m_textRenderer.isInitialized()) return;
    
    if (!renderer) return;

    SDL_Rect boxRect = {m_posX, m_posY, m_width, m_height};
    SDL_Rect innerRect = {m_posX + m_style.paddingX, m_posY + m_style.paddingY,
                         m_width - 2 * m_style.paddingX, m_height - 2 * m_style.paddingY};

    // Ensure inner rect has positive dimensions
    if (innerRect.w < 1) innerRect.w = 1;
    if (innerRect.h < 1) innerRect.h = 1;


    // 1. Draw Background
    if (m_style.drawBackground) {
        SDL_SetRenderDrawColor(renderer, m_style.bgColor.r, m_style.bgColor.g, m_style.bgColor.b, m_style.bgColor.a);
        SDL_RenderFillRect(renderer, &boxRect);
    }

   // 2. Draw Border (changes color if focused)
    if (m_style.drawBorder) {
        SDL_Color borderColor = m_hasFocus ? m_style.focusedBorderColor : m_style.borderColor;
        SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        SDL_RenderDrawRect(renderer, &boxRect);
    }

    // --- Rendering Text Area ---
    // Store old clip rect to restore later
    SDL_Rect oldClipRect;
    SDL_RenderGetClipRect(renderer, &oldClipRect);
    // Apply clipping to the inner area
    SDL_RenderSetClipRect(renderer, &innerRect);

    int textW = 0, textH = 0;
    SDL_Texture* textTexture = nullptr;
    if (!m_displayText.empty()) {
        textTexture = m_textRenderer.renderTextToTexture(m_displayText, m_style.textColor, m_fontSize, textW, textH);
    }

    int textRenderX = innerRect.x - m_scrollX; // Apply scroll offset
    int textRenderY = innerRect.y + (innerRect.h - textH) / 2; // Center vertically

    // 3. Draw Selection Background (if selection exists)
    if (m_hasFocus && m_selectionStart != -1) {
        int startX = getTextXPosition(std::min(m_selectionStart, m_cursorPos));
        int endX = getTextXPosition(std::max(m_selectionStart, m_cursorPos));
        SDL_Rect selectionRect = { textRenderX + startX, innerRect.y, endX - startX, innerRect.h };

        SDL_SetRenderDrawColor(renderer, m_style.selectionBgColor.r, m_style.selectionBgColor.g, m_style.selectionBgColor.b, m_style.selectionBgColor.a);
        //SDL_SetRenderDrawColor(renderer, 20, 20, 255, 255);
        // Enable blending for semi-transparent selection
        SDL_BlendMode oldBlendMode;
        SDL_GetRenderDrawBlendMode(renderer, &oldBlendMode);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &selectionRect);
        SDL_SetRenderDrawBlendMode(renderer, oldBlendMode); // Restore blend mode
    }

    // 4. Draw Text (using cached texture)
    if (textTexture) {
         SDL_Rect textDst = { textRenderX, textRenderY, textW, textH };
         SDL_RenderCopy(renderer, textTexture, nullptr, &textDst);
         // Texture is managed by TextRenderer cache, no need to destroy
    }

    // 5. Draw Cursor (if focused and visible)
    if (m_hasFocus && m_isCursorVisible) {
        int cursorScreenX = textRenderX + getTextXPosition(m_cursorPos);
        SDL_SetRenderDrawColor(renderer, m_style.cursorColor.r, m_style.cursorColor.g, m_style.cursorColor.b, m_style.cursorColor.a);
        SDL_RenderDrawLine(renderer, cursorScreenX, innerRect.y, cursorScreenX, innerRect.y + innerRect.h -1);
    }

    // Restore clipping rectangle
    SDL_RenderSetClipRect(renderer, nullptr);
}


void InputBox::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        handleMouseInput(event);
    }
    // Add MOUSEMOTION, MOUSEBUTTONUP for selection dragging

    if (m_hasFocus) { // Only process keyboard/text if focused
        if (event.type == SDL_KEYDOWN) {
            handleKeyDown(event.key.keysym.sym, static_cast<SDL_Keymod>(event.key.keysym.mod));
        } else if (event.type == SDL_TEXTINPUT) {
            handleTextInput(event.text.text);
        }
    }
}

// --- Implement focus/unfocus ---
void InputBox::focus() {
    if (!m_hasFocus) {
        m_hasFocus = true;
        m_isCursorVisible = true; // Show cursor immediately
        m_cursorBlinkTimer = 0.0f;
        SDL_StartTextInput(); // Tell SDL we want text input events
        SDL_SetTextInputRect(nullptr); // Let SDL manage IME popup position (or specify rect {m_posX, m_posY,...})
        std::cout << "InputBox focused." << std::endl;
    }
}
void InputBox::unfocus() {
    if (m_hasFocus) {
        m_hasFocus = false;
        m_selectionStart = -1; // Clear selection on unfocus
        SDL_StopTextInput(); // Tell SDL we don't want text events anymore
         std::cout << "InputBox unfocused." << std::endl;
    }
}



// Example sketch for handleKeyDown:
void InputBox::handleKeyDown(SDL_Keycode key, SDL_Keymod mod) {
    bool shift = (mod & KMOD_SHIFT);
    bool ctrl = (mod & KMOD_CTRL); // Add KMOD_GUI for Mac Cmd key

     // Reset blink on key press
    m_isCursorVisible = true;
    m_cursorBlinkTimer = 0.0f;

    switch (key) {
        case SDLK_LEFT:
            // TODO: Handle selection with shift
            if(m_cursorPos > 0) m_cursorPos--;
            if (!shift) m_selectionStart = -1; // Clear selection if not shift-moving
            break;
        case SDLK_RIGHT:
             // TODO: Handle selection with shift
            if (m_cursorPos < m_text.length()) m_cursorPos++;
             if (!shift) m_selectionStart = -1;
            break;
        case SDLK_BACKSPACE:
            if (m_selectionStart != -1) {
                deleteSelection();
            } else if (m_cursorPos > 0) {
                m_text.erase(m_cursorPos - 1, 1);
                m_cursorPos--;
                 if (onTextChanged) onTextChanged(m_text); updateDisplayText();
            }
            break;
        case SDLK_DELETE:
            if (m_selectionStart != -1) {
                deleteSelection();
            } else if (m_cursorPos < m_text.length()) {
                m_text.erase(m_cursorPos, 1);
                 if (onTextChanged) onTextChanged(m_text); updateDisplayText();
            }
            break;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
             if (onEnterPressed) onEnterPressed(m_text);
            // Maybe unfocus(); ?
            break;
        case SDLK_c: // Copy
             if (ctrl && m_selectionStart != -1) {
                 // TODO: Copy selected text to clipboard SDL_SetClipboardText()
             }
            break;
        case SDLK_v: // Paste
            if (ctrl && SDL_HasClipboardText()) {
                 // TODO: Delete selection, get SDL_GetClipboardText(), insert at cursor, update cursor
                 // Careful with memory from SDL_GetClipboardText! Free it.
                 // if (onTextChanged) onTextChanged(m_text); updateDisplayText();
             }
            break;
        // TODO: Add Home, End, Ctrl+Left/Right, Select All (Ctrl+A), Cut (Ctrl+X)
        default:
            break; // Ignore other keys handled by SDL_TEXTINPUT
    }
    clampCursorAndScroll(); // Ensure cursor/scroll is valid after move/delete
}

// Example sketch for handleTextInput:
void InputBox::handleTextInput(const char* inputText) {
     deleteSelection(); // Delete selection before inserting
     m_text.insert(m_cursorPos, inputText);
     m_cursorPos += strlen(inputText); // Basic handling, need UTF-8 awarenes ideally
     if (onTextChanged) onTextChanged(m_text);
     updateDisplayText();
     clampCursorAndScroll();
}

// --- Implement other helpers ---
void InputBox::updateDisplayText() {
    if (m_isPassword) {
        m_displayText = std::string(m_text.length(), '*');
    } else {
        m_displayText = m_text;
    }
}






// --- Add this function definition ---
void InputBox::handleMouseInput(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            // Check if click is inside the INPUT BOX's bounds
            bool inside = (mouseX >= m_posX && mouseX <= (m_posX + m_width) &&
                           mouseY >= m_posY && mouseY <= (m_posY + m_height));

            if (inside) {
                // --- Clicked Inside ---
                if (!m_hasFocus) {
                    focus(); // Gain focus, call SDL_StartTextInput() etc.
                }
                // Reset cursor blink on click
                m_isCursorVisible = true;
                m_cursorBlinkTimer = 0.0f;

                // Calculate the character index based on the mouse click position
                int clickedIndex = getIndexFromXCoord(mouseX);
                m_cursorPos = clickedIndex;

                // Handle selection start:
                // If Shift is held, extend selection from existing m_selectionStart.
                // Otherwise, start a new selection (or clear it).
                SDL_Keymod modState = SDL_GetModState();
                if (!(modState & KMOD_SHIFT)) {
                    // Not holding Shift: Start new selection at cursor pos
                    m_selectionStart = m_cursorPos;
                    // We are now potentially dragging for selection
                    // You might need a boolean member like m_isDragging = true; here
                } else {
                    // Holding Shift: Keep existing m_selectionStart, just move m_cursorPos
                    // If no selection start exists yet, treat as normal click
                    if (m_selectionStart == -1) {
                        m_selectionStart = m_cursorPos;
                    }
                    // Need to set m_isDragging = true; here too if extending
                }

                // Make sure cursor position is valid and scroll if needed
                clampCursorAndScroll();

            } else {
                // --- Clicked Outside ---
                if (m_hasFocus) {
                    unfocus(); // Lose focus
                }
            }
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        // --- Mouse Dragging for Selection ---
        // Check if Left button is held down (state, not event type)
        Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
        if (m_hasFocus && (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && m_selectionStart != -1 /* && m_isDragging */) {
            // Need m_isDragging flag set in MOUSEBUTTONDOWN to confirm we started drag inside
            int mouseX = event.motion.x;
            m_cursorPos = getIndexFromXCoord(mouseX);
            clampCursorAndScroll(); // Scroll view while dragging if needed
             // Reset blink on drag interaction
            m_isCursorVisible = true;
            m_cursorBlinkTimer = 0.0f;
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            // Stop dragging
            // m_isDragging = false; // Reset dragging flag

            // Optional: If selection start and end are the same after dragging, clear selection
            if (m_hasFocus && m_selectionStart == m_cursorPos) {
                 m_selectionStart = -1;
            }
        }
    }
}

// --- Make sure you also implement these helpers used by handleMouseInput ---
int dummy;
// Calculates character index from mouse X coordinate (relative to box start)
int InputBox::getIndexFromXCoord(int mouseX) {
    if (!m_textRenderer.isInitialized()) return 0;

    // Adjust mouseX to be relative to the text's render start position (including scroll)
    int relativeX = mouseX - (m_posX + m_style.paddingX - m_scrollX);
    if (relativeX < 0) return 0; // Clicked before text start

    int currentW = 0;
    int totalW = 0;
    // Measure total width first to handle clicks beyond the end
    m_textRenderer.measureText(m_displayText, m_fontSize, totalW, dummy);
    if (relativeX >= totalW) return m_displayText.length(); // Clicked after text end


    // Iterate through characters (handle UTF-8 correctly!)
    // Simple ASCII/single-byte version for brevity:
    for (int i = 0; i < m_displayText.length(); ++i) {
        int charW = 0;
        // Measure current char (or substring for better accuracy with kerning)
        std::string sub = m_displayText.substr(0, i + 1);
        m_textRenderer.measureText(sub, m_fontSize, charW, dummy);

        // Find the character index where the click falls within its width
        // Check midpoint of character width maybe? Or just the start? Let's use start.
        if (relativeX < charW) {
            // More accurate: check midpoint
             int prevW = (i > 0) ? currentW : 0;
             if (relativeX < (prevW + (charW - prevW) / 2)) {
                 return i;
             } else {
                 return i + 1;
             }
        }
        currentW = charW; // Store width up to this point for next iteration's midpoint check
    }

    return m_displayText.length(); // Should have returned earlier if clicked within text
}

// Calculates the screen X coordinate (relative to text start) for a given character index
int InputBox::getTextXPosition(int charIndex) {
     if (!m_textRenderer.isInitialized() || charIndex <= 0) return 0;
     if (charIndex >= m_displayText.length()) {
         // Measure full string width
         int totalW = 0;
         m_textRenderer.measureText(m_displayText, m_fontSize, totalW, dummy);
         return totalW;
     }
     // Measure substring up to the index
     int subW = 0;
     std::string sub = m_displayText.substr(0, charIndex);
     m_textRenderer.measureText(sub, m_fontSize, subW, dummy);
     return subW;
}

// Ensures cursor is within text bounds and scrolls horizontally if needed
void InputBox::clampCursorAndScroll() {
     // 1. Clamp cursor position
     m_cursorPos = std::max(0, std::min((int)m_text.length(), m_cursorPos));

     // 2. Clamp selection start (if active)
     if (m_selectionStart != -1) {
         m_selectionStart = std::max(0, std::min((int)m_text.length(), m_selectionStart));
     }

     // 3. Update Horizontal Scroll (m_scrollX)
     if (!m_textRenderer.isInitialized()) return;

     int innerWidth = m_width - 2 * m_style.paddingX;
     if (innerWidth <= 0) return; // Nothing to scroll within

     // Get the pixel X position of the cursor relative to text start
     int cursorPixelX = getTextXPosition(m_cursorPos);

     // If cursor is left of the visible area's start
     if (cursorPixelX < m_scrollX) {
         m_scrollX = cursorPixelX;
     }
     // If cursor is right of the visible area's end
     else if (cursorPixelX > m_scrollX + innerWidth) {
         m_scrollX = cursorPixelX - innerWidth;
     }

     // Ensure scroll doesn't go beyond text bounds
     int totalTextW = 0;
     m_textRenderer.measureText(m_displayText, m_fontSize, totalTextW, dummy);
     int maxScrollX = std::max(0, totalTextW - innerWidth);
     m_scrollX = std::max(0, std::min(m_scrollX, maxScrollX));
}


// In src/ui/InputBox.cpp


// --- Add this function definition ---
void InputBox::deleteSelection() {
    // Check if there is an active selection (start != -1)
    // and if the start and current cursor position are different
    if (m_selectionStart != -1 && m_selectionStart != m_cursorPos) {

        // Determine the actual start index and the number of characters to erase
        int selection_idx_1 = m_selectionStart;
        int selection_idx_2 = m_cursorPos;

        int start_index = std::min(selection_idx_1, selection_idx_2);
        int end_index = std::max(selection_idx_1, selection_idx_2);
        int count = end_index - start_index;

        // Ensure indices are valid before erasing
        if (start_index >= 0 && count > 0 && start_index + count <= m_text.length()) {
            // Erase the selected portion from the underlying text string
            m_text.erase(start_index, count);

            // Move the cursor to the beginning of where the selection was
            m_cursorPos = start_index;

            // Update the text displayed (handles password masking)
            updateDisplayText();

            // Notify via callback that text changed
            if (onTextChanged) {
                onTextChanged(m_text);
            }

            // Reset cursor blink state as interaction occurred
            m_isCursorVisible = true;
            m_cursorBlinkTimer = 0.0f;

            // Ensure cursor position and scroll are updated correctly
            // Although clampCursorAndScroll will be called again after text insertion/key handling,
            // it might be good to call it here too for consistency, though perhaps redundant.
            // clampCursorAndScroll(); // Optional call here
        }
        // else: Log error? Indices were somehow invalid.

        // Always clear the selection after attempting deletion
        m_selectionStart = -1;
    }
    // If no selection exists (m_selectionStart == -1 or start==end), do nothing.
}



// ... Getters/Setters ...
std::string InputBox::getText() const { return m_text; }
void InputBox::setText(const std::string& text) {
    m_text = text;
    m_cursorPos = m_text.length(); // Move cursor to end
    m_selectionStart = -1;
    m_scrollX = 0;
    if (onTextChanged) onTextChanged(m_text);
    updateDisplayText();
    clampCursorAndScroll();
}


} // namespace XenUI