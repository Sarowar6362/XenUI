//XenonUI/src/graphics/sdl2/TextRenderer.cpp
#include "TextRenderer.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <sstream>

#include <fstream> // For checking file existence
#include <vector>
// Add platform-specific includes for font finding here
#ifdef __linux__
    #include <fontconfig/fontconfig.h> // Requires linking -lfontconfig
#elif defined(__APPLE__)
    // #include <CoreText/CoreText.h> // Requires Objective-C or C wrapper
#elif defined(_WIN32)
    // #include <dwrite.h> // Requires COM and linking dwrite.lib
    // #include <windows.h> // For other potential methods
#endif

// ... getInstance(), Constructor, Destructor, getFont(), clearCache(), createCacheKey() ...
// ... renderText(), measureText(), etc. (These use m_fontPath via getFont()) ...





TextRenderer::TextRenderer() : m_renderer(nullptr), m_initialized(false) {}

TextRenderer::~TextRenderer() {
    clearCache();
    
    TTF_Quit();  // Quit SDL_ttf when done
}

TextRenderer& TextRenderer::getInstance() {
    static TextRenderer instance;
    return instance;
}




// void TextRenderer::init(SDL_Renderer* renderer, const std::string& fontPath) {
//         if (m_initialized) return;
//         if (TTF_Init() == -1) {
//             std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
//             m_initialized = false;
//             return;
//         }
//         m_renderer = renderer;
//         m_fontPath = fontPath; // Store path
//         m_initialized = true;  // TTF_Init is in main
//         std::cout << "TextRenderer initialized. Font path: " << m_fontPath << std::endl;
// }
    








// ADD: getFont implementation
TTF_Font* TextRenderer::getFont(int fontSize) {
    if (!m_initialized || m_fontPath.empty() || fontSize <= 0) return nullptr;
    auto it = m_fontsBySize.find(fontSize);
    if (it != m_fontsBySize.end()) {
        return it->second; // Found in map
    }
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
       
    }
    
    // Not found, load it
    TTF_Font* font = TTF_OpenFont(m_fontPath.c_str(), fontSize);
    
if (!font) {
    std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
    return nullptr;
}

    if (!font) {
        std::cerr << "getFont: Failed to open font '" << m_fontPath << "' size " << fontSize << ". Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    m_fontsBySize[fontSize] = font; // Add to map
    return font;
}


// CHANGE: Updated renderText
void TextRenderer::renderText(const std::string& text, int x, int y, SDL_Color color, int fontSize) {
    if (!m_initialized) return;
    int texW = 0, texH = 0;
    // Get/create texture using the internal caching method (which now uses size)
    SDL_Texture* texture = renderTextToTexture(text, color, fontSize, texW, texH);
    if (!texture) return;
    SDL_Rect dstRect = { x, y, texW, texH };
    SDL_RenderCopy(m_renderer, texture, nullptr, &dstRect);
}

// CHANGE: Updated renderTextToTexture (used by renderText)
SDL_Texture* TextRenderer::renderTextToTexture(const std::string& text, SDL_Color color, int fontSize, int& outW, int& outH) {
    if (!m_initialized || text.empty()) {/*... set 0 return null ...*/}

    std::string key = createCacheKey(text, fontSize); // Use new key function
    auto cacheIt = m_textureCache.find(key);
    if (cacheIt != m_textureCache.end()) {
        outW = cacheIt->second.width; outH = cacheIt->second.height;
        return cacheIt->second.texture; // Return cached
    }

   TTF_Font* font = getFont(fontSize); // Use getFont
   if (!font) return nullptr;

   SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
   // ... (Surface check) ...
   SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
   // ... (Texture check) ...
   if (texture) {
       outW = surface->w; outH = surface->h;
       m_textureCache[key] = {texture, outW, outH}; // Add to cache with new key
   }
   SDL_FreeSurface(surface);
   return texture;
}


SDL_Renderer* TextRenderer::getRenderer() const {
    return m_renderer;
}


// void TextRenderer::clearCache() {
//     for (auto& pair : textCache) {
//         if (pair.second) {
//             SDL_DestroyTexture(pair.second);
//         }
//     }
//     textCache.clear();
// }

// CHANGE: Updated clearCache
void TextRenderer::clearCache() {
    // Clear textures
    for (auto const& [key, val] : m_textureCache) {
        if (val.texture) SDL_DestroyTexture(val.texture);
    }
    m_textureCache.clear();
    // Clear fonts
    for (auto const& [size, font] : m_fontsBySize) {
        if (font) TTF_CloseFont(font);
    }
    m_fontsBySize.clear();
    std::cout << "Cleared TextRenderer cache (textures and fonts)." << std::endl;
}


// ADD: createCacheKey implementation
std::string TextRenderer::createCacheKey(const std::string& text, int fontSize) {
    std::stringstream ss;
    ss << text << '|' << fontSize; // Include size in key
    return ss.str();
}


// In TextRenderer.cpp

// NEW: For immediate-mode rendering (no caching, caller destroys texture)
SDL_Texture* TextRenderer::renderTextImmediateToTexture(const std::string& text, SDL_Color color, int fontSize, int& outW, int& outH) {
    // Initialize output parameters
    outW = 0;
    outH = 0;

    if (!m_initialized || text.empty() || fontSize <= 0) {
        std::cerr << "Error: TextRenderer not ready or invalid input for immediate render." << std::endl;
        return nullptr;
    }

    // *** Use getFont() to get the correct font size ***
    TTF_Font* font = getFont(fontSize);
    if (!font) {
        // getFont already prints an error if it fails
        return nullptr;
    }

    // *** Render using the obtained font ***
    // Using Blended for better quality with alpha, consider UTF8 for broader text support
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Error creating immediate surface: " << TTF_GetError() << std::endl;
        // Do NOT close the font here, it's managed by the m_fontsBySize map
        return nullptr;
    }

    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (texture) {
        // Get dimensions if texture creation succeeded
        outW = surface->w;
        outH = surface->h;
    } else {
        std::cerr << "Error creating immediate texture: " << SDL_GetError() << std::endl;
        // Texture is nullptr, outW/outH remain 0
    }

    // Free the temporary surface, we don't need it anymore
    SDL_FreeSurface(surface);

    // Return the newly created texture.
    // IMPORTANT: The caller of this function is responsible for calling
    // SDL_DestroyTexture() on the returned texture when done.
    return texture;
}

// CHANGE: Updated measureText/getTextSize
void TextRenderer::measureText(const std::string& text, int fontSize, int& w, int& h) {
    if (!m_initialized || text.empty()) {/*... set 0 ...*/ return; }
    TTF_Font* font = getFont(fontSize); // Use getFont
    if (!font) {/*... set 0 ...*/ return; }
    if (TTF_SizeUTF8(font, text.c_str(), &w, &h) != 0) { /*...*/ }
    // NO TTF_CloseFont here!
}

SDL_Point TextRenderer::getTextSize(const std::string& text, int fontSize) {
    SDL_Point size = {0, 0};
    measureText(text, fontSize, size.x, size.y);
    return size;
}





// *** Platform-specific System Font Search ***
std::string TextRenderer::findSystemFont(const std::vector<std::string>& preferredFamilies) {
    std::vector<std::string> familiesToSearch = preferredFamilies;
    // Add common fallbacks if preferred list is empty or fails
    if (familiesToSearch.empty()) {
        familiesToSearch = {"DejaVu Sans", "Arial", "Helvetica", "Verdana", "Calibri", "Roboto"}; // Example defaults
    }

    std::cout << "TextRenderer: Searching for system fonts..." << std::endl;

#ifdef __linux__
    // --- Linux (fontconfig) ---
    std::cout << "TextRenderer: Using fontconfig (Linux)..." << std::endl;
    FcInit();
    FcPattern* pattern = FcPatternCreate();
    FcObjectSet* os = FcObjectSetBuild(FC_FILE, (char*)0); // We only need the file path
    std::string foundPath = "";

    for (const std::string& family : familiesToSearch) {
        // Clean previous family constraint if any
        FcPatternRemove(pattern, FC_FAMILY, 0);
        // Add current family constraint
        FcPatternAddString(pattern, FC_FAMILY, (const FcChar8*)family.c_str());
        // Request a scalable format (like TTF/OTF)
        FcPatternAddBool(pattern, FC_SCALABLE, FcTrue);
        // Match pattern
        FcConfigSubstitute(nullptr, pattern, FcMatchPattern);
        FcDefaultSubstitute(pattern);
        FcResult result;
        FcPattern* match = FcFontMatch(nullptr, pattern, &result);

        if (match) {
            FcChar8* filepath = nullptr;
            if (FcPatternGetString(match, FC_FILE, 0, &filepath) == FcResultMatch) {
                foundPath = (const char*)filepath;
                std::cout << "TextRenderer: Found system font '" << foundPath << "' for family '" << family << "'" << std::endl;
            }
            FcPatternDestroy(match); // Destroy the matched pattern
        }
        if (!foundPath.empty()) break; // Stop searching once found
    }

    FcPatternDestroy(pattern); // Destroy the search pattern
    // FcFini(); // Calling FcFini() might interfere if app uses fontconfig elsewhere? Research needed. Typically called once at app exit.
    return foundPath;

#elif defined(__APPLE__)
    // --- macOS (Core Text - Placeholder) ---
    std::cout << "TextRenderer: System font search placeholder (macOS)." << std::endl;
    // TODO: Implement using CTFontManager, CTFontDescriptorCreateWithNameAndSize, CTFontCopyAttribute(kCTURLAttribute) -> CFURL -> Get File Path
    return ""; // Placeholder

#elif defined(_WIN32)
    // --- Windows (DirectWrite or GDI - Placeholder) ---
    std::cout << "TextRenderer: System font search placeholder (Windows)." << std::endl;
    // TODO: Implement using DirectWrite (preferred) or GDI (EnumFontFamiliesEx) to find font path by family name.
    return ""; // Placeholder

#else
    std::cout << "TextRenderer: System font search not implemented for this platform." << std::endl;
    return ""; // Not implemented
#endif
}


// *** Find Bundled Fallback Font ***
std::string TextRenderer::findBundledFallbackFont() {
    std::cout << "TextRenderer: Searching for bundled fallback font..." << std::endl;
#ifdef XENUI_FALLBACK_FONT_PATH // Check if path was embedded via CMake
    std::string embeddedPath = XENUI_FALLBACK_FONT_PATH;
    std::cout << "TextRenderer: Checking embedded path: " << embeddedPath << std::endl;
    // Basic check if file exists (use std::filesystem for better check if C++17 available)
    std::ifstream f(embeddedPath.c_str());
    if (f.good()) {
        std::cout << "TextRenderer: Found fallback font at embedded path." << std::endl;
        return embeddedPath;
    } else {
         std::cerr << "TextRenderer: Embedded fallback font path not found or not readable: " << embeddedPath << std::endl;
    }
#else
    std::cout << "TextRenderer: No embedded fallback font path defined." << std::endl;
    // Add other fallback strategies if needed (e.g., check relative to DLL/framework bundle)
    // Example Linux standard path check (less reliable than embedded)
    #ifdef __linux__
        const char* standardPath = "/usr/share/fonts/truetype/XenUI/DejaVuSans.ttf"; // Example path
         std::ifstream f_std(standardPath);
         if(f_std.good()) {
            std::cout << "TextRenderer: Found fallback font at standard path: " << standardPath << std::endl;
            return standardPath;
         }
    #endif
#endif

    std::cerr << "TextRenderer: Bundled fallback font could not be located." << std::endl;
    return "";
}


// *** MODIFIED init Implementation ***
void TextRenderer::init(SDL_Renderer* renderer, const std::vector<std::string>& preferredFamilies) {
    if (m_initialized) return;
    m_renderer = renderer;

    // 1. Try finding a system font
    m_fontPath = findSystemFont(preferredFamilies);

    // 2. If system font not found, try the bundled fallback
    if (m_fontPath.empty()) {
        m_fontPath = findBundledFallbackFont();
    }

    // 3. Check if we finally have a valid path
    if (m_fontPath.empty()) {
        std::cerr << "ERROR: TextRenderer init failed. Could not find any suitable system or fallback font!" << std::endl;
        m_initialized = false; // Stay uninitialized
        return; // Fail initialization
    }

    // 4. Optional: Verify the chosen font can be loaded (e.g., try loading default size)
    // TTF_Font* testFont = getFont(16); // Test load size 16
    // if (!testFont) {
    //      std::cerr << "ERROR: TextRenderer init failed. Could not load the chosen font file: " << m_fontPath << std::endl;
    //      m_fontPath = ""; // Clear the invalid path
    //      m_initialized = false;
    //      return; // Fail initialization
    // }
     // We don't need to keep testFont pointer explicitly, getFont cached it.

    m_initialized = true;
    std::cout << "TextRenderer initialized successfully. Using font: " << m_fontPath << std::endl;
}

