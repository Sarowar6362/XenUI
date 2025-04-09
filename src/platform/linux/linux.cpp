// src/platform/linux/linux.cpp
// dont remove first two comments
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstring>  // For memcpy
#include <string>
#include "../../graphics/sdl2/TextRenderer.h"
#include "../../ui/Label.h"
#include "../../ui/Shape.h"
#include "../../ui/Button.h"
#include "../../utils/WindowUtil.h"
#include "../../utils/Anchor.h"  // Include Anchor header
#include "../../utils/Position.h" // *** ADDED ***

// src/platform/linux/linux.cpp
// ... other includes ...



// Global text renderer instance
TextRenderer& textRenderer = TextRenderer::getInstance();
std::vector<Label> labels;
std::vector<Button> buttons; // Store buttons in a vector
std::vector<XenUI::Rectangle> shapes; // Example

// This method calculates the position based on anchor
// *** TODO: Refactor Label setup to use PositionParams ***

void setupLabels() {
    
    
    labels.emplace_back("Another Label", 
        XenUI::PositionParams::Anchored(XenUI::Anchor::BOTTOM_RIGHT, -10, -20), 
        1.0f, 
        SDL_Color{255, 255, 255, 255}, true);
    
    //  std::cout << "Label setup needs refactoring to use PositionParams.\n";
}


// *** MODIFIED setupButtons ***
void setupButtons() {
    ButtonStyle style1;
    style1.textColor = {255, 255, 255, 255};
    style1.bgColor = {10, 200, 100, 255};
    

    // Use PositionParams::Anchored for the first button
    buttons.emplace_back(
        "Click Me",                     // text
        XenUI::PositionParams::Anchored(XenUI::Anchor::TOP_LEFT, 50, 50), // Position
        style1,                         // style
        []() { std::cout << "Button 1 Clicked!" << std::endl; } // onClick
    );

    ButtonStyle style2;
    style2.textColor = {255, 255, 255, 255};
    style2.bgColor = {50, 50, 50, 255};
    

    // Use PositionParams::Anchored for the second button
    buttons.emplace_back(
        "Second Button",                // text
        XenUI::PositionParams::Anchored(XenUI::Anchor::BOTTOM_RIGHT, -50, -50), // Position
        style2,                         // style
        []() { std::cout << "Button 2 Clicked!" << std::endl; } // onClick
    );
}

// *** MODIFIED render function ***
void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    // Drawing Retained Mode objects
    // *** TODO: Refactor shapes to use PositionParams ***
                                                        //(x, y), width, height, color(rgba)
     XenUI::Rectangle rect(XenUI::PositionParams::Absolute(50, 50), 200, 50, SDL_Color{50, 150, 255, 255});
                                                                        //(position, x, y), width, DynamicHeight, color(rgba) //Dynamic Height means height changes according to window height                                                               
     XenUI::Rectangle rect1(XenUI::PositionParams::Anchored(XenUI::Anchor::BOTTOM_CENTER, 0, 0), 200, -1, SDL_Color{50, 150, 255, 255}); //simply pass -1 in width or height or in both for dynamic height
    XenUI::Circle circle(XenUI::PositionParams::Anchored(XenUI::Anchor::TOP_RIGHT, -50, 50), 60, SDL_Color{255, 80, 80, 255});
    rect.draw(renderer);
    rect1.draw(renderer);
    circle.draw(renderer);

    // *** TODO: Refactor retained Labels to use PositionParams and draw correctly ***
    for (Label& label : labels) {
       label.draw(); // Assuming Label::draw exists and uses calculated position
    }

     // Retained buttons draw
     for (Button& button : buttons) {
        button.draw(renderer);
    }



    // === Immediate mode elements ===







    // Immediate mode labels - TODO: Refactor XenUI::Label to use PositionParams
            //text                                                   //(x, y), scale, color, cacheText
     XenUI::Label("Immediate Label A", XenUI::PositionParams::Absolute(10, 100), 1.0f, SDL_Color{200, 200, 50, 255}, true);
                   //text                                                //(position, x, y), scale, color, cacheText
    XenUI::Label("FPS: 60", XenUI::PositionParams::Anchored(XenUI::Anchor::TOP_RIGHT, -10, 10), 1.0f, SDL_Color{255, 255, 255, 255}, true);

   

    // Immediate mode buttons using PositionParams
    ButtonStyle blackstyle;
    blackstyle.textColor = {255, 255, 255, 255};
    blackstyle.bgColor = {0, 0, 0, 255};
    if (XenUI::Button("ok_btn", "Ok (Absolute)", XenUI::PositionParams::Absolute(200, 300), blackstyle)) {
        std::cout << "OK Button Pressed\n";
    }

    ButtonStyle redStyle;
    redStyle.bgColor = {180, 30, 30, 255};
    redStyle.textColor = {255, 255, 255, 255};
    redStyle.paddingX = 20;
    redStyle.paddingY = 10;
    // Anchored Exit Button
    if (XenUI::Button("exit_btn", "Exit (Anchored)", XenUI::PositionParams::Anchored(XenUI::Anchor::BOTTOM_CENTER, 0, -30), redStyle)) {
        std::cout << "Exit Button Pressed\n";
        exit(0);
        // Consider setting a flag `running = false;` instead of exit(0);
    }

    SDL_RenderPresent(renderer);
}

// setup function - Ensure TextRenderer init still happens
void setup(SDL_Window* window, SDL_Renderer* renderer) {
    XenUI::SetWindow(window); // Set window reference for GetWindowSize

    // *** Fix Font Path for Cross-Platform ***
    const char* fontName = "DejaVuSans.ttf"; // Or your bundled font name
    std::string fontPath = "";
    char* basePath_c = SDL_GetBasePath();
     if (!basePath_c) {
         std::cerr << "Error getting base path: " << SDL_GetError() << std::endl;
         // Fallback: Try a common system path (still not ideal)
         #ifdef _WIN32
             fontPath = "C:/Windows/Fonts/arial.ttf"; // Example Windows fallback
         #elif __APPLE__
              fontPath = "/Library/Fonts/Arial.ttf"; // Example macOS fallback
         #else // Linux / other POSIX
             fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"; // Original path as last resort
         #endif
         std::cerr << "Warning: Using fallback font path: " << fontPath << std::endl;
     } else {
         std::string basePath(basePath_c);
         SDL_free(basePath_c); // IMPORTANT: Free the memory!
         #ifdef _WIN32
             fontPath = basePath + "assets\\fonts\\" + fontName; // Assumes font is in assets/fonts
         #else
             fontPath = basePath + "assets/fonts/" + fontName; // Assumes font is in assets/fonts
         #endif
          std::cout << "Attempting to load font from: " << fontPath << std::endl;
     }


    std::cout << "Initializing text renderer...\n";
    if (!textRenderer.isInitialized()) {
        textRenderer.init(renderer, fontPath, 20); // Use constructed path
    }

    if (!textRenderer.isInitialized()) {
        std::cerr << "TEXT RENDERER INIT FAILED! Check font path: " << fontPath << std::endl;
        // Maybe try loading a guaranteed system font as a last resort? Or exit.
        exit(1);
    }

    setupLabels(); // Needs refactoring internally
    setupButtons(); // Now uses PositionParams
}

// main function (event loop structure remains largely the same)
int main(int argc, char** argv) {
    // ... SDL Init, Window, Renderer creation ...



    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1; // Exit if SDL fails
    }

    // 2. Create the Window
    //    This line declares the 'window' variable and assigns the created window to it.
    SDL_Window* window = SDL_CreateWindow(
        "Your Window Title",           // Title
        SDL_WINDOWPOS_CENTERED,        // X position
        SDL_WINDOWPOS_CENTERED,        // Y position
        800,                           // Width
        600,                           // Height
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN // Flags
    );

    // Check if window creation was successful
    if (window == nullptr) { // Use nullptr for pointer comparison
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit(); // Clean up SDL
        return 1;   // Exit
    }

    // 3. Create the Renderer
    //    This line declares the 'renderer' variable and assigns the created renderer to it.
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,                       // The window to render into
        -1,                           // Rendering driver index (-1 for first available)
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Flags (VSync recommended)
    );

    // Check if renderer creation was successful
    if (renderer == nullptr) { // Use nullptr for pointer comparison
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window); // Clean up the window we created
        SDL_Quit();                // Clean up SDL
        return 1;                  // Exit
    }


     if (TTF_Init() == -1) { // Ensure TTF is initialized
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
            } else if (event.type == SDL_WINDOWEVENT) {
                 if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    // std::cout << "Window Resized!" << std::endl;
                    // Recalculate positions for retained anchored elements
                    for (Button& button : buttons) {
                         button.recalculatePosition(); // Calls internal CalculateFinalPosition
                    }
                    for (Label& label : labels) { label.recalculatePosition(); } // TODO
                    for (XenUI::Shape& shape : shapes) { shape.recalculatePosition(); } // TODO
                 }
            }

            // Pass events to retained mode buttons
            for (Button& button : buttons) {
                button.handleEvent(event);
            }
            // TODO: Pass events to retained Labels, Shapes if they need it
        } // end event polling

        render(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    // ... Cleanup (DestroyRenderer, DestroyWindow, TTF_Quit, SDL_Quit) ...
    textRenderer.clearCache(); // Good practice before TTF_Quit if using caching heavily

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}