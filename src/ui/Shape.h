#pragma once
#include <SDL2/SDL.h>

namespace XenUI {

    class Shape {
    public:
        virtual void draw(SDL_Renderer* renderer) = 0;
        virtual ~Shape() = default;
    };

    class Rectangle : public Shape {
    public:
        Rectangle(int x, int y, int width, int height, SDL_Color color);
        void draw(SDL_Renderer* renderer) override;

    private:
        int m_x, m_y, m_width, m_height;
        SDL_Color m_color;
    };

    class Circle : public Shape {
    public:
        Circle(int centerX, int centerY, int radius, SDL_Color color);
        void draw(SDL_Renderer* renderer) override;

    private:
        int m_x, m_y, m_radius;
        SDL_Color m_color;
    };

}
