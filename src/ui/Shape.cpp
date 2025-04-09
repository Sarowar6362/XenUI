#include "Shape.h"
#include "../utils/WindowUtil.h"
namespace XenUI {

    // === RECTANGLE ===
    #include "../utils/WindowUtil.h" // For GetWindowSize

    Rectangle::Rectangle(PositionParams posParams, int width, int height, SDL_Color color)
        : m_posParams(posParams), m_width(width), m_height(height), m_color(color),
          m_dynamicWidth(width == -1), m_dynamicHeight(height == -1) {
        recalculatePosition();
    }
    
    void Rectangle::recalculatePosition() {
        SDL_Point windowSize = GetWindowSize();
        int finalWidth = m_dynamicWidth ? windowSize.x : m_width;
        int finalHeight = m_dynamicHeight ? windowSize.y : m_height;
    
        SDL_Point pos = CalculateFinalPosition(m_posParams, finalWidth, finalHeight);
        m_x = pos.x;
        m_y = pos.y;
    
        // Store real values for drawing
        m_width = finalWidth;
        m_height = finalHeight;
    }
    
    

    void Rectangle::draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
        SDL_Rect rect = {m_x, m_y, m_width, m_height};
        SDL_RenderFillRect(renderer, &rect);
    }

    // === CIRCLE ===
    Circle::Circle(PositionParams posParams, int radius, SDL_Color color)
        : m_posParams(posParams), m_radius(radius), m_color(color) {
        recalculatePosition();
    }

    void Circle::recalculatePosition() {
        // We treat circle as a square for anchoring; width and height = diameter
        int size = m_radius * 2;
        SDL_Point pos = CalculateFinalPosition(m_posParams, size, size);
        m_x = pos.x + m_radius; // Adjust to center
        m_y = pos.y + m_radius;
    }

    void Circle::draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
        for (int w = 0; w < m_radius * 2; w++) {
            for (int h = 0; h < m_radius * 2; h++) {
                int dx = m_radius - w;
                int dy = m_radius - h;
                if ((dx * dx + dy * dy) <= (m_radius * m_radius)) {
                    SDL_RenderDrawPoint(renderer, m_x + dx, m_y + dy);
                }
            }
        }
    }

}
