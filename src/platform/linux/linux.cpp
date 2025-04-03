//src/platform/linux/linux.cpp
//dont remove first two comments
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstring>  // For memcpy
#include "../../graphics/sdl2/TextRenderer.h"
#include "../../ui/Label.h"

// Global text renderer instance
TextRenderer& textRenderer = TextRenderer::getInstance();

void render(SDL_Renderer* renderer) {
    // Clear screen with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render test label
    static Label testLabel("Xenon UI Label", 50, 500, 1.0f);
    static bool firstRun = true;

    if (firstRun) {
        SDL_Color blueColor = {255, 255, 255, 255};
        testLabel.setColor(blueColor);
        firstRun = false;
    }
    testLabel.draw();

    // Present the renderer (display the result)
    SDL_RenderPresent(renderer);
}

void setup(SDL_Window* window, SDL_Renderer* renderer) {
    const char* fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    
    std::cout << "Initializing text renderer...\n";
    if (!textRenderer.isInitialized()) {
        textRenderer.init(renderer, fontPath, 30);  // Pass SDL_Renderer here
    }

    if (!textRenderer.isInitialized()) {
        std::cerr << "TEXT RENDERER INIT FAILED!\n";
        exit(1);
    }
}


int main(int argc, char** argv) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL INIT ERROR: " << SDL_GetError() << "\n";
        return 1;
    }

    // Create a resizable window
SDL_Window* window = SDL_CreateWindow("Xenon UI", 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
    800, 600, 
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

// Check if window creation was successful
if (!window) {
std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
exit(1);
}

// Create the renderer
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
if (!renderer) {
std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
exit(1);
}

    // Initialize SDL_ttf (for font rendering)
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf INIT ERROR: " << TTF_GetError() << "\n";
        return 1;
    }

    // Setup text renderer
    setup(window, renderer);

    // Main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Render scene
        render(renderer);

        // Delay to control frame rate
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup and quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
