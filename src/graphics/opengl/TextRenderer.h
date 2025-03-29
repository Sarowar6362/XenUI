//src/graphics/opengl/TextRenderer.h
//dont remove first two coomments
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <GL/glew.h>
#include <map>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>  // Add this at the top of your file



// ... rest of your header ...
class TextRenderer {
public:
    static TextRenderer& getInstance();
    bool isInitialized() const { return m_initialized; }  // Add this
    void checkGLErrors(const char* context);  // Add this
    void init(const std::string& fontPath, unsigned int fontSize);
    void renderText(const std::string& text, float x, float y, float scale, 
                   const float color[4] = nullptr);
    
    void setProjection(int width, int height);
    
private:
    TextRenderer();
    ~TextRenderer();
    
    struct Character {
        GLuint textureID;
        int width, height;
        int bearingX, bearingY;
        unsigned int advance;
    };

    FT_Library m_ft;
    FT_Face m_face;
    std::map<char, Character> m_characters;
    GLuint m_vao, m_vbo, m_shaderProgram;
    bool m_initialized = false;
    int m_windowWidth = 0, m_windowHeight = 0;
    
    void setupBuffers();
    void compileShaders();
};

#endif