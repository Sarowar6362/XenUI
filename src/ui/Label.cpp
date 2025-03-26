#include "Label.h"
#include <X11/Xutil.h>
#include <Xft/Xft.h>
#include <iostream>

Label::Label(Display* display, Window parent, int x, int y, const std::string& text)
    : display(display), parent(parent), x(x), y(y), text(text) {
    
    // Try loading a fallback font
    font = XftFontOpenName(display, DefaultScreen(display), "DejaVu Sans-12:size=12");
    if (!font) {
        font = XftFontOpenName(display, DefaultScreen(display), "Arial-12:size=12");
        if (!font) {
            std::cerr << "Failed to load any font\n";
            exit(1);
        }
    }

    // Allocate color (white)
    if (!XftColorAllocName(display, DefaultVisual(display, 0), DefaultColormap(display, 0),
                          "white", &color)) {
        std::cerr << "Failed to allocate color\n";
        exit(1);
    }

    // Initialize XftDraw
    xftDraw = XftDrawCreate(display, parent, DefaultVisual(display, 0), DefaultColormap(display, 0));
    if (!xftDraw) {
        std::cerr << "Failed to create XftDraw\n";
        exit(1);
    }
}

Label::~Label() {
    XftColorFree(display, DefaultVisual(display, 0), DefaultColormap(display, 0), &color);
    XftFontClose(display, font);
    XftDrawDestroy(xftDraw);
}

void Label::setText(const std::string& newText) {
    text = newText;
}


void Label::draw() {
    XGlyphInfo extents;
    XftTextExtentsUtf8(display, font, (XftChar8*)text.c_str(), text.length(), &extents);

    // Calculate baseline position (y + font ascent)
    int baseline_y = y + (extents.height - extents.y); 

    // Draw the text directly without clearing
    XftDrawStringUtf8(xftDraw, &color, font, x, baseline_y, 
                      (XftChar8*)text.c_str(), text.length());
    XFlush(display); // Ensure X11 commands are processed
}