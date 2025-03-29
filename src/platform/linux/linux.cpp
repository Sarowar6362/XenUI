//src/platform/linux/linux.cpp
//dont remove first two coomments
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <cstring>  // For memcpy
#include "../../graphics/opengl/TextRenderer.h"
#include "../../ui/Label.h"

TextRenderer& textRenderer = TextRenderer::getInstance();

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up OpenGL state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render test label
    static Label testLabel("Xenon UI Label", 50.0f, 500.0f, 1.0f);
    static bool firstRun = true;
    
    if (firstRun) {
        float blueColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        testLabel.setColor(blueColor);
        firstRun = false;
    }
    testLabel.draw();

    glutSwapBuffers();
}

void setup() {
    const char* fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    
    
    std::cout << "Initializing text renderer...\n";
    if (!textRenderer.isInitialized()) {
        textRenderer.init(fontPath, 30);
        textRenderer.setProjection(800, 600);
    }

    if (!textRenderer.isInitialized()) {
        std::cerr << "TEXT RENDERER INIT FAILED!\n";
        exit(1);
    }
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Text Rendering Demo");

    // Initialize GLEW with error checking
    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK) {
        std::cerr << "GLEW INIT ERROR: " << glewGetErrorString(glewErr) << "\n";
        return 1;
    }

    // Print OpenGL context info
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n"
              << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n"
              << "Renderer: " << glGetString(GL_RENDERER) << "\n";

    // Check for required OpenGL features
    GLint redFormatSupport;
    glGetInternalformativ(GL_TEXTURE_2D, GL_RED, GL_INTERNALFORMAT_SUPPORTED, 1, &redFormatSupport);
    if (!redFormatSupport) {
        std::cerr << "ERROR: GL_RED texture format not supported!\n";
        return 1;
    }

    // Setup and verify
    setup();

    // Check for OpenGL errors
    GLenum glErr;
    while ((glErr = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL ERROR: " << glErr << "\n";
    }

    // Set up callbacks
    glutDisplayFunc(render);
    glutIdleFunc([]() { glutPostRedisplay(); }); // Continuous rendering

    // Start main loop
    glutMainLoop();

    return 0;
}