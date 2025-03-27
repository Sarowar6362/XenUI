#include "../../ui/Label.h"
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <iostream>

Display* display;
Window window;
GLXContext glContext;
bool running = true;
Label* label = nullptr;

void createWindow(int width, int height) {
    display = XOpenDisplay(nullptr);
    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    // Use OpenGL-compatible visual
    GLint visualAttr[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 24, None };
    XVisualInfo* vi = glXChooseVisual(display, screen, visualAttr);
    Colormap colormap = XCreateColormap(display, root, vi->visual, AllocNone);

    XSetWindowAttributes swa;
    swa.colormap = colormap;
    swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;

    window = XCreateWindow(display, root, 0, 0, width, height, 0, vi->depth, 
                           InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XMapWindow(display, window);
    XStoreName(display, window, "XenUI");

    glContext = glXCreateContext(display, vi, nullptr, GL_TRUE);
    glXMakeCurrent(display, window, glContext);
    glViewport(0, 0, width, height);
}

void render() {
    // Clear OpenGL buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glXSwapBuffers(display, window); // Show OpenGL content

    // Draw label AFTER swapping buffers
    if (label) label->draw();
}

void eventLoop() {
    XEvent event;
    while (running) {
        XNextEvent(display, &event);

        if (event.type == DestroyNotify || event.type == KeyPress) {
            running = false;
        } else if (event.type == Expose || event.type == ConfigureNotify) {
            if (event.type == ConfigureNotify) {
                glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
            }
            render(); // Redraw on resize/expose
        }
    }
}

int main() {
    createWindow(800, 600);
    label = new Label(display, window, 50, 50, "Hello, XenUI!");
    render(); // Initial draw
    eventLoop();
    return 0;
}