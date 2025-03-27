#include "Label.h"
#include <X11/Xutil.h>
#include <Xft/Xft.h>
#include <iostream>
// #include <ft2build.h>
// #include FT_FREETYPE_H
// #include <X11/Xft/Xft.h>
Label::Label(Display* display, Window parent, int x, int y, const std::string& text)
    : display(display), parent(parent), x(x), y(y), text(text) {
    
    // Load font (fallback to default if needed)
    font = XftFontOpenName(display, DefaultScreen(display), "DejaVu Sans:size=12");
    if (!font) {
        std::cerr << "ERROR: Failed to load font. Trying fixed font...\n";
        font = XftFontOpenName(display, DefaultScreen(display), "fixed");
        if (!font) {
            std::cerr << "FATAL: No fonts available\n";
            exit(1);
        }
    }

    // Allocate white color
    if (!XftColorAllocName(display, DefaultVisual(display, 0), 
                           DefaultColormap(display, 0), "white", &color)) {
        std::cerr << "FATAL: Failed to allocate color\n";
        exit(1);
    }

    // Draw directly to the window (no Pixmap)
    xftDraw = XftDrawCreate(display, parent, 
                            DefaultVisual(display, 0), 
                            DefaultColormap(display, 0));
}

Label::~Label() {
    XftColorFree(display, DefaultVisual(display, 0), DefaultColormap(display, 0), &color);
    XftFontClose(display, font);
    XftDrawDestroy(xftDraw);
}

void Label::draw() {
    XGlyphInfo extents;
    XftTextExtents8(display, font, (XftChar8*)text.c_str(), text.length(), &extents);

    XClearArea(display, parent, x, y - font->ascent, 
               extents.width,  // Use extents.width from XftTextExtents8
               font->ascent + font->descent, False);
    
    XftDrawStringUtf8(xftDraw, &color, font, x, y + font->ascent, 
                     (XftChar8*)text.c_str(), text.length());
    XFlush(display);
}