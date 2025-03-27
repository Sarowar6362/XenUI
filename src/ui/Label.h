#ifndef XENUI_LABEL_H
#define XENUI_LABEL_H

#include <string>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

class Label {
public:
    Label(Display* display, Window parent, int x, int y, const std::string& text);
    ~Label();
    void draw();

private:
    Display* display;
    Window parent;
    int x, y;
    std::string text;
    XftFont* font;
    XftColor color;
    XftDraw* xftDraw;
};

#endif // XENUI_LABEL_H