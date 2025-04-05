// src/platform/linux/linux.cpp
// dont remove first two comments
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstring>  // For memcpy
#include "../../graphics/sdl2/TextRenderer.h"
#include "../../ui/Label.h"
#include "../../ui/Button.h"

// Global text renderer instance


SDL_Window* window  = nullptr;
SDL_Renderer* renderer = nullptr;

TextRenderer& textRenderer = TextRenderer::getInstance();
std::vector<Label> labels;  
std::vector<Button> buttons; // Store buttons in a vector

void setupLabels() {
    labels.emplace_back("First Label 56", 10, 10, 1.0f, SDL_Color{255, 255, 255, 255});
    labels.emplace_back("Second Label", 200, 50, 1.0f, SDL_Color{0, 255, 0, 255});
}

void setupButtons() {
    buttons.emplace_back("Click Me again and again", 150, 200, 130, 50, SDL_Color{110, 200, 50, 255},
                         SDL_Color{255, 255, 255, 255}, []() {
                             std::cout << "Button 1 Clicked!" << std::endl;
                         });

    buttons.emplace_back("Exit", 300, 200, 120, 50, SDL_Color{255, 0, 50, 255},
                         SDL_Color{255, 255, 255, 255}, []() {
                             std::cout << "Exit Button Clicked!" << std::endl;
                             exit(0); // Close the application
                         });
}

void setup(SDL_Window* window, SDL_Renderer* renderer) {
    const char* fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";

    std::cout << "Initializing text renderer...\n";
    if (!textRenderer.isInitialized()) {
        textRenderer.init(renderer, fontPath, 20);
    }

    if (!textRenderer.isInitialized()) {
        std::cerr << "TEXT RENDERER INIT FAILED!\n";
        exit(1);
    }

    setupLabels();
    setupButtons();
}

int sdl_init(void) {

 if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL INIT ERROR: " << SDL_GetError() << "\n";
        return 1;
    }

    window = SDL_CreateWindow("Xenon UI",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf INIT ERROR: " << TTF_GetError() << "\n";
        return 1;
    }

    setup(window, renderer);
     return 0;

}

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 80, 100, 100, 255);
    SDL_RenderClear(renderer);

    for (Label& label : labels) {
        label.draw();
    }

    for (Button& button : buttons) {
        button.draw(renderer);  // ✅ Pass renderer
    }

    SDL_RenderPresent(renderer);
}



int main(int argc, char** argv) {
    if(!sdl_init()) return 1;
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // Pass events to buttons
            for (Button& button : buttons) {
                button.handleEvent(event);
            }
        }

        render(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
