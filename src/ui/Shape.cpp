#include "Shape.h"

namespace XenUI {

    // === RECTANGLE ===
    Rectangle::Rectangle(int x, int y, int width, int height, SDL_Color color)
        : m_x(x), m_y(y), m_width(width), m_height(height), m_color(color) {}

    void Rectangle::draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
        SDL_Rect rect = {m_x, m_y, m_width, m_height};
        SDL_RenderFillRect(renderer, &rect);
    }

    // === CIRCLE ===
    Circle::Circle(int centerX, int centerY, int radius, SDL_Color color)
        : m_x(centerX), m_y(centerY), m_radius(radius), m_color(color) {}

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
