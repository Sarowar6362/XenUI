#pragma once
#include <SDL2/SDL.h>
#include "Anchor.h"
#include "Position.h"

namespace XenUI {

    class Shape {
    public:
        virtual void draw(SDL_Renderer* renderer) = 0;
        virtual void recalculatePosition() = 0;
        virtual ~Shape() = default;
    };

    class Rectangle : public Shape {
        public:
            // Pass -1 for dynamic width or height
            Rectangle(PositionParams posParams, int width, int height, SDL_Color color);
            void draw(SDL_Renderer* renderer) override;
            void recalculatePosition() override;
        
        private:
            PositionParams m_posParams;
            int m_width, m_height;
            int m_x, m_y;
            SDL_Color m_color;
        
            bool m_dynamicWidth, m_dynamicHeight;
        };
        

    class Circle : public Shape {
    public:
        Circle(PositionParams posParams, int radius, SDL_Color color);
        void draw(SDL_Renderer* renderer) override;
        void recalculatePosition() override;

    private:
        PositionParams m_posParams;
        int m_radius;
        int m_x, m_y;
        SDL_Color m_color;
    };

}
