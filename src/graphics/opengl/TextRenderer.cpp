//src/graphics/opengl/TextRenderer.cpp
//dont remove first two coomments
#include "TextRenderer.h"
#include <iostream>

const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main() {    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
}
)glsl";

TextRenderer& TextRenderer::getInstance() {
    static TextRenderer instance;
    return instance;
}

TextRenderer::TextRenderer() {
    // Initialize in init()
}

TextRenderer::~TextRenderer() {
    if (m_initialized) {
        FT_Done_Face(m_face);
        FT_Done_FreeType(m_ft);
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
        glDeleteProgram(m_shaderProgram);
    }
}

void TextRenderer::compileShaders() {
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Shader program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    ///aded
    GLint success;
    char infoLog[512];
    
    // Vertex shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex shader compile error:\n" << infoLog << "\n";
    }

    // Fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment shader compile error:\n" << infoLog << "\n";
    }

    // Program linking
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program link error:\n" << infoLog << "\n";
    }
}

void TextRenderer::checkGLErrors(const char* context) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL ERROR (" << context << "): " << err << "\n";
    }
}

void TextRenderer::init(const std::string& fontPath, unsigned int fontSize) {
    if (m_initialized) return;

    // 1. Verify font file exists
    std::ifstream fontStream(fontPath);
if (!fontStream.good()) {
    std::cerr << "ERROR: Font file not found at " << fontPath << "\n";
    std::cerr << "Install DejaVu fonts: sudo apt-get install fonts-dejavu\n";
    exit(1);
}
fontStream.close();

    // 2. Initialize FreeType
    if (FT_Init_FreeType(&m_ft)) {
        std::cerr << "FREETYPE INIT ERROR\n";
        return;
    }

    // 3. Load face
    if (FT_New_Face(m_ft, fontPath.c_str(), 0, &m_face)) {
        std::cerr << "FONT LOAD ERROR: " << fontPath << "\n";
        FT_Done_FreeType(m_ft);
        return;
    }

    // 4. Set size and alignment
    FT_Set_Pixel_Sizes(m_face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    checkGLErrors("Font setup");

    // 5. Load glyphs
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) {
            std::cerr << "WARNING: Failed to load Glyph '" << c << "'\n";
            continue;
        }
    
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED,
            m_face->glyph->bitmap.width,
            m_face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE,
            m_face->glyph->bitmap.buffer
        );
    
        // CORRECTED VALIDATION (using m_face instead of face)
        GLint texWidth, texHeight;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
        
        if (texWidth != m_face->glyph->bitmap.width || texHeight != m_face->glyph->bitmap.rows) {
            std::cerr << "TEXTURE SIZE MISMATCH for '" << c << "': " 
                      << "Expected " << m_face->glyph->bitmap.width << "x" << m_face->glyph->bitmap.rows
                      << ", got " << texWidth << "x" << texHeight << "\n";
        }       // Verify texture
        // GLint width = 0;
        // glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        // if (width != m_face->glyph->bitmap.width) {
        //     std::cerr << "TEXTURE CREATION FAILED FOR CHAR: " << c << "\n";
        // }

        // Set parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character
        m_characters[c] = {
            texture,
            static_cast<int>(m_face->glyph->bitmap.width),
            static_cast<int>(m_face->glyph->bitmap.rows),
            static_cast<int>(m_face->glyph->bitmap_left),
            static_cast<int>(m_face->glyph->bitmap_top),
            static_cast<unsigned int>(m_face->glyph->advance.x)
        };
    }

    // 6. Cleanup FreeType
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_ft);

    // 7. Setup buffers and shaders
    setupBuffers();
    compileShaders();
    
    m_initialized = true;
    checkGLErrors("Init complete");
}
void TextRenderer::setupBuffers() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::setProjection(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;

    std::cout << "Projection set to: " << width << "x" << height << "\n";
}

void TextRenderer::renderText(const std::string& text, float x, float y, float scale, const float color[4]) {
    if (!m_initialized) return;

    // Activate corresponding render state
    glUseProgram(m_shaderProgram);
    
    // Set common uniforms
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_windowWidth), 
                                     0.0f, static_cast<float>(m_windowHeight));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 
                      1, GL_FALSE, glm::value_ptr(projection));
    
    // Set text color
    float textColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    if (color) std::copy(color, color + 4, textColor);
    glUniform4f(glGetUniformLocation(m_shaderProgram, "textColor"), 
               textColor[0], textColor[1], textColor[2], textColor[3]);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_vao);

    // Iterate through all characters
    for (auto c = text.begin(); c != text.end(); c++) {
        Character ch = m_characters[*c];

        float xpos = x + ch.bearingX * scale;
        float ypos = y - (ch.height - ch.bearingY) * scale;
        float w = ch.width * scale;
        float h = ch.height * scale;

        // Update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        
        // Update VBO content
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursor
        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}