#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>  // Required for std::ifstream
#include "../../graphics/opengl/TextRenderer.h"

TextRenderer& textRenderer = TextRenderer::getInstance();

void render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up proper blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render test text
    float whiteColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    textRenderer.renderText("Test Texture", 50, 500, 1.0f, whiteColor);
    
    glutSwapBuffers();
}

void setup() {
    // Verify font path
    const char* fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    std::cout << "Attempting to load font from: " << fontPath << std::endl;
    
    // Check if font file exists
    std::ifstream fontFile(fontPath);
    if (!fontFile.good()) {
        std::cerr << "ERROR: Font file not found at " << fontPath << std::endl;
        std::cerr << "Try installing with: sudo apt-get install fonts-dejavu" << std::endl;
        exit(1);
    }
    fontFile.close();

    textRenderer.init(fontPath, 48);
    textRenderer.setProjection(800, 600);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Text Rendering");

    // Initialize GLEW properly
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        return 1;
    }

    setup();
    glutDisplayFunc(render);
    glutMainLoop();

    return 0;
}