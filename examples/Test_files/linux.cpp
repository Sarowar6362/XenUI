// src/platform/linux/linux.cpp
// dont remove first two comments
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <cstring>  // For memcpy
#include <string>
#include <chrono>
#include "XenUI/TextRenderer.h"
#include "XenUI/Label.h"
#include "XenUI/InputBox.h"
#include "XenUI/Shape.h"
#include "XenUI/Button.h"
#include "XenUI/WindowUtil.h"
#include "XenUI/Anchor.h"  // Include Anchor header
#include "XenUI/Position.h" // *** ADDED ***

// src/platform/linux/linux.cpp
// ... other includes ...



// Global text renderer instance
TextRenderer& textRenderer = TextRenderer::getInstance();
std::vector<Label> labels;
std::vector<Button> buttons; // Store buttons in a vector
std::vector<XenUI::InputBox> Inputs; // Store Inputboxs in a vector
std::vector<XenUI::Rectangle> shapes; // Example

// This method calculates the position based on anchor
// *** TODO: Refactor Label setup to use PositionParams ***

//test variables
int fps = 60 ;

void setupLabels() {
    
    
    labels.emplace_back("Fps : 60", 
        XenUI::PositionParams::Anchored(XenUI::Anchor::CENTER, -100, -30), 
         23, //textSize
        SDL_Color{255, 255, 255, 255});



    labels.emplace_back("Another Label", 
        XenUI::PositionParams::Anchored(XenUI::Anchor::BOTTOM_RIGHT, -10, -20), 
         30, //textSize
        SDL_Color{255, 255, 255, 255});
    
    //  std::cout << "Label setup needs refactoring to use PositionParams.\n";
}


// *** MODIFIED setupButtons ***
void setupButtons() {
    ButtonStyle style1;
    style1.textColor = {255, 255, 255, 255};
    style1.bgColor = {10, 200, 100, 255};
 
    // Use PositionParams::Anchored for the first button
    buttons.emplace_back(
        "Add immediate + retained mode Fps",                     // text
        XenUI::PositionParams::Anchored(XenUI::Anchor::TOP_LEFT, 50, 370), // Position
        style1,                         // style
        []() { 
            fps++;
            std::string UpdatedText = "Fps : "+ std::to_string(fps);
            labels[0].setText(UpdatedText);
            
            std::cout << "Button 1 Clicked!" << std::endl; 
        }, // onClick
        30
    );




    buttons.emplace_back(
        "Click Me",                     // text
        XenUI::PositionParams::Anchored(XenUI::Anchor::TOP_LEFT, 50, 50), // Position
        style1,                         // style
        []() { std::cout << "Button 1 Clicked!" << std::endl; }, // onClick
        40
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




//  // Declare one InputBox instance
//  XenUI::InputBox myInputBox(
//     XenUI::PositionParams::Absolute(100, 200), // Position using PositionParams
//     "Edit Me!",                                // Initial text
//     300,                                       // Width hint
//     18,  // Font size
//     style                                 
//     // You can also pass a custom 
//    //InputBoxStyle style, 
void myinput()
{
    XenUI::InputBoxStyle style;
    Inputs.emplace_back(
        XenUI::PositionParams::Absolute(100, 200), 
        "Edit Me!",
        300,
        18

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

   


// Inside void render(SDL_Renderer* renderer) in linux.cpp
// ... clear screen ...
// ... draw buttons, labels ...

//myInputBox.draw(renderer); // Draw the input box



// ... SDL_RenderPresent ...



    // *** TODO: Refactor retained Labels to use PositionParams and draw correctly ***
    for (Label& label : labels) {
       label.draw(); // Assuming Label::draw exists and uses calculated position
    }

     // Retained buttons draw
     for (Button& button : buttons) {
        button.draw(renderer);
    }

    //If using a vector:
for (auto& box : Inputs) {
    box.draw(renderer);
}
    



    // === Immediate mode elements ===





    // Immediate mode labels - TODO: Refactor XenUI::Label to use PositionParams
            //text                                                   //(x, y), fontSize, color
     XenUI::Label("Immediate Label A", XenUI::PositionParams::Absolute(10, 100), 30, SDL_Color{200, 200, 50, 255});
                   //text                                                //(position, x, y), textSize, color
    XenUI::Label("FPS: 60", XenUI::PositionParams::Anchored(XenUI::Anchor::TOP_RIGHT, -10, 10), 30, SDL_Color{255, 255, 255, 255});

   

 


    // Immediate mode buttons using PositionParams
    ButtonStyle blackstyle;
    blackstyle.textColor = {255, 255, 255, 255}; //r,g,b,a
    blackstyle.bgColor = {0, 0, 0, 255}; //black background
                                                                                                //x, y
    if (XenUI::Button("ok_btn", "Add immediate mode Fps only", XenUI::PositionParams::Absolute(200, 300), blackstyle, 30)) { //fontSize = 30 
        fps++;
        std::cout << "OK Button Pressed\n";
    }
    //test example, for fps counter
    std::string fpsText = "Fps : " + std::to_string(fps);

    XenUI::Label(fpsText, XenUI::PositionParams::Anchored(XenUI::Anchor::TOP_RIGHT, -10, 40), 25, SDL_Color{0, 255, 0, 255} );

    ButtonStyle redStyle;
    redStyle.bgColor = {180, 30, 30, 255};
    redStyle.textColor = {255, 255, 255, 255};
    redStyle.paddingX = 20;
    redStyle.paddingY = 10;
    // Anchored Exit Button
    if (XenUI::Button("exit_btn", "Exit (Anchored)", XenUI::PositionParams::Anchored(XenUI::Anchor::BOTTOM_CENTER, 0, -30), redStyle)) { //default fontSize = 16
        std::cout << "Exit Button Pressed\n";
        exit(0);
        // Consider setting a flag `running = false;` instead of exit(0);
    }

    SDL_RenderPresent(renderer);
}

// setup function - Ensure TextRenderer init still happens
// In linux.cpp (Application Code)
void setup(SDL_Window* window, SDL_Renderer* renderer) {
    XenUI::SetWindow(window); // Set window reference for GetWindowSize

    std::cout << "Initializing text renderer (framework finds font)...\n";
    if (!textRenderer.isInitialized()) {
        // Framework's init now handles font discovery
        // Optionally pass preferences:
    //     std::vector<std::string> prefs = {"Noto Sans", "DejaVu Sans"};
    //   textRenderer.init(renderer, prefs);

        // Simple call, using default preferences internal to TextRenderer
        textRenderer.init(renderer);
    }

    if (!textRenderer.isInitialized()) {
        std::cerr << "CRITICAL ERROR: TEXT RENDERER FAILED TO INITIALIZE - NO FONT FOUND." << std::endl;
        // Consider a graceful exit or fallback UI without text
        exit(1);
    }

    setupLabels();
    setupButtons();
    myinput();
}

// main function (event loop structure remains largely the same)

int main(int argc, char** argv) {

    // 1. Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 2. Create the Window (as you have it)
    SDL_Window* window = SDL_CreateWindow(
        "Xenon UI",                    // Window title
        SDL_WINDOWPOS_CENTERED,        // Initial x position
        SDL_WINDOWPOS_CENTERED,        // Initial y position
        800,                           // Initial Width
        600,                           // Initial Height
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN // Flags (Resizable is important)
    );

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // *** === ADD THIS: Set Minimum Window Size === ***
    const int MIN_WINDOW_WIDTH = 400;  // Example minimum width
    const int MIN_WINDOW_HEIGHT = 200; // Example minimum height
    SDL_SetWindowMinimumSize(window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    std::cout << "Minimum window size set to " << MIN_WINDOW_WIDTH << "x" << MIN_WINDOW_HEIGHT << std::endl;
    // *** ======================================== ***

    // 3. Create the Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 4. Initialize TTF
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        // ... cleanup ...
        return 1;
    }

    // 5. Call Setup
    setup(window, renderer);

    // 6. Main Loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            // *** === ADD THIS: Fullscreen Toggle Logic === ***
            else if (event.type == SDL_KEYDOWN) { // Check for key presses
                // Toggle fullscreen on F11 press
                if (event.key.keysym.sym == SDLK_F11) {
                    // Get current window flags to check if already fullscreen
                    Uint32 current_flags = SDL_GetWindowFlags(window);
                    bool is_fullscreen = (current_flags & SDL_WINDOW_FULLSCREEN_DESKTOP) || (current_flags & SDL_WINDOW_FULLSCREEN);

                    if (is_fullscreen) {
                        // Turn fullscreen OFF
                        SDL_SetWindowFullscreen(window, 0);
                        std::cout << "Exited fullscreen mode." << std::endl;
                    } else {
                        // Turn fullscreen ON (using desktop resolution)
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        std::cout << "Entered fullscreen (desktop) mode." << std::endl;
                    }
                }
            }
            // *** ======================================= ***
             else if (event.type == SDL_WINDOWEVENT) {
                 if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    // std::cout << "Window Resized!" << std::endl;
                    // Recalculate positions for retained anchored elements
                    for (Button& button : buttons) {
                         button.recalculatePosition(); // Calls internal CalculateFinalPosition
                    }
                    for (Label& label : labels) { label.recalculatePosition(); } // TODO
                    // for (XenUI::Shape& shape : shapes) { shape.recalculatePosition(); } // TODO
                    for(XenUI::InputBox& box : Inputs) { box.recalculatePosition();}
                 }
            }
        
            // Pass events to retained mode buttons
            for (Button& button : buttons) {
                button.handleEvent(event);
            }
            for (XenUI::InputBox& box : Inputs) {
                box.handleEvent(event);
            }
            // //myInputBox.handleEvent(event); 
            // TODO: Pass events to retained Labels, Shapes if they need it
        } // end event polling



// Inside the while(running) loop in main()

// // --- Calculate deltaTime (Example using chrono) ---
// static auto lastTime = std::chrono::high_resolution_clock::now();
// auto currentTime = std::chrono::high_resolution_clock::now();
// std::chrono::duration<float> dtChrono = currentTime - lastTime;
// lastTime = currentTime;
// float deltaTime = dtChrono.count();
// for (auto& box : Inputs) {
//     box.update(deltaTime);
// }
// --- End deltaTime Calculation ---

// ... event loop happens above ...

// Call update for the input box
//myInputBox.update(deltaTime);

//If using a vector:


// ... Call render(renderer) ...





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
