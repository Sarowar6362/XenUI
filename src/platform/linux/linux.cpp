#include "../../ui/Label.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <iostream>

// Global variables
Display* display;
Window window;
GLXContext glContext;
bool running = true;

// Declare label as global
Label* label = nullptr;

void createWindow(int width, int height) {
    display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Failed to open X display\n";
        exit(1);
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    GLint visualAttr[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    XVisualInfo* vi = glXChooseVisual(display, screen, visualAttr);
    if (!vi) {
        std::cerr << "No suitable visual found\n";
        exit(1);
    }

    Colormap colormap = XCreateColormap(display, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = colormap;
    swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;

    window = XCreateWindow(display, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
                           CWColormap | CWEventMask, &swa);
    XMapWindow(display, window);
    XStoreName(display, window, "XenUI - OpenGL");

    glContext = glXCreateContext(display, vi, nullptr, GL_TRUE);
    glXMakeCurrent(display, window, glContext);
}

void render(Label* label) {
    // OpenGL: Clear and swap buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glXSwapBuffers(display, window); // Swap to show the cleared buffer

    // Draw the label on top of the OpenGL content
    if (label) {
        label->draw();
    }
}

void eventLoop() {
    XEvent event;
    while (running) {
        // Block until an event arrives (no continuous rendering)
        XNextEvent(display, &event); 

        if (event.type == DestroyNotify) running = false;
        if (event.type == KeyPress) running = false;

        // Redraw only when the window is exposed or resized
        if (event.type == Expose || event.type == ConfigureNotify) {
            render(label);
        }
    }
}

void cleanup() {
    glXMakeCurrent(display, None, nullptr);
    glXDestroyContext(display, glContext);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    // Clean up label object
    delete label;
}

int main() {
    createWindow(800, 600);
    label = new Label(display, window, 50, 100, "Hello, XenUI!");
    render(label); // Initial draw
    eventLoop();
    cleanup();
}