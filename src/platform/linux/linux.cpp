// src/platform/linux/linux.cpp
// dont remove first two comments
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstring>  // For memcpy
#include "../../graphics/sdl2/TextRenderer.h"
#include "../../ui/Label.h"
#include "../../ui/Shape.h"
#include "../../ui/Button.h"

// Global text renderer instance
TextRenderer& textRenderer = TextRenderer::getInstance();
std::vector<Label> labels;  
std::vector<Button> buttons; // Store buttons in a vector

void setupLabels() {
    labels.emplace_back("First Label 56", 10, 10, 1.0f, SDL_Color{255, 255, 255, 255});
    labels.emplace_back("Second Label", 200, 50, 1.0f, SDL_Color{0, 255, 0, 255});
}

void setupButtons() {
    // Create a ButtonStyle for the first button
    ButtonStyle style1;
    style1.bgColor = {110, 200, 50, 255};
    style1.textColor = {255, 255, 255, 255};

    buttons.emplace_back("Click Me and not working", 150, 200, 130, 50, style1, []() {
        std::cout << "Button 1 Clicked!" << std::endl;
    });

    // Create a ButtonStyle for the second button
    ButtonStyle style2;
    style2.bgColor = {255, 0, 50, 255};
    style2.textColor = {255, 255, 255, 255};

    buttons.emplace_back("Exit and exit button", 450, 200, 120, 50, style2, []() {
        std::cout << "Exit Button Clicked!" << std::endl;
        exit(0); // Close the application
    });
}


void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 80, 100, 100, 255);
    SDL_RenderClear(renderer);

    XenUI::Rectangle rect(50, 50, 200, 100, SDL_Color{50, 150, 255, 255});
    XenUI::Circle circle(300, 150, 60, SDL_Color{255, 80, 80, 255});

    rect.draw(renderer);
    circle.draw(renderer);

    for (Label& label : labels) {
        label.draw();
    }


    // Immediate mode labels
    XenUI::Label("Immediate Label A", 10, 100, 1.0f,  SDL_Color{200, 200, 50, 255}, false); 
    XenUI::Label("FPS: 60", 700, 10, 1.0f,  SDL_Color{255, 255, 255, 255});

    for (Button& button : buttons) {
        button.draw(renderer);  // ✅ Pass renderer
    }


                    //text, x, y, width, height, color(rgb)
    // === Immediate mode buttons ===

    // Default style, auto-sizing
    ButtonStyle blackstyle;
    blackstyle.textColor = {255, 255, 255, 255};
    blackstyle.bgColor = {0, 0, 0, 255};
    if (XenUI::Button("ok_btn", "Ok and this button is expanding", 200, 300, blackstyle)) {
        std::cout << "OK Button Pressed\n";
    }

    // Custom style
    ButtonStyle redStyle;
    redStyle.bgColor = {180, 30, 30, 255};
    redStyle.textColor = {255, 255, 255, 255};
    redStyle.paddingX = 20;
    redStyle.paddingY = 10;

    

    if (XenUI::Button("exit_btn", "Exit-this button is also expanding", 200, 360, redStyle)) {
        std::cout << "Exit Button Pressed\n";
        exit(0);
    }

    


    SDL_RenderPresent(renderer);
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

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL INIT ERROR: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Xenon UI",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf INIT ERROR: " << TTF_GetError() << "\n";
        return 1;
    }

    setup(window, renderer);

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
