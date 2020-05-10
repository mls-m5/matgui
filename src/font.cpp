/*
 * Font.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/font.h"

#ifdef USE_BITMAP_FONT
#include "matgui/bitmapfont.h"

typedef void FontType;

#else

#include <SDL2/SDL_ttf.h>

typedef TTF_Font FontType;

#endif

#include "matgui/draw.h"
#include "matgui/shaderprogram.h"
#include <list>

namespace MatGui {

struct FontDescriptionStruct {
    std::string filename;
    int size;
    FontType *font;
};

class FontData {
public:
    FontType *font = nullptr;
};

// Unoptimized function to draw text, consider to use fontview instead to buffer
// data
void renderText(const FontType *font,
                GLubyte r,
                GLubyte g,
                GLubyte b,
                double x,
                double y,
                double z,
                const std::string &text,
                int alignment);

namespace {

std::list<FontDescriptionStruct> loadedFonts;
bool isInitialized = false;
std::string defaultFontPath = "font/Ubuntu-R.ttf";

} // namespace

void renderText(const FontType *font,
                GLubyte r,
                GLubyte g,
                GLubyte b,
                double x,
                double y,
                double z,
                const std::string &text,
                int alignment) {
    if (!font)
        return;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create some variables.
#ifdef USE_BITMAP_FONT
    (void)r;
    (void)g;
    (void)b;
    auto data = getFontDataVector(text);
    auto messageSurface = new BitmapFont(text);
#else
    SDL_Color color = {r, g, b};
    SDL_Surface *messageSurface = TTF_RenderUTF8_Blended(
        const_cast<TTF_Font *>(font), text.c_str(), color);
#endif

    unsigned Texture = 0;

    // Generate an OpenGL 2D texture from the SDL_Surface*.
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE); // This is important for webgl otherwhise
                                       // only a black box is drawn
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE); // for non pow2 size textures

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 MATGUI_INTERNAL_TEXTURE_FORMAT,
                 messageSurface->w,
                 messageSurface->h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 messageSurface->pixels);

    drawTextureRect({x + messageSurface->w * alignment / 2, y, z},
                    0,
                    messageSurface->w,
                    messageSurface->h,
                    Texture,
                    DrawStyle::CenterOrigo);

    // Clean up.

    glDeleteTextures(1, &Texture);
#ifndef USE_BITMAP_FONT
    SDL_FreeSurface(messageSurface);
#endif
}

Font::Font(const std::string filename, int size) : Font() {
#ifdef USE_BITMAP_FONT
    (void)filename;
    (void)size;
    _data->font = (void *)-1; // some random non-null value
#else
    FontDescriptionStruct *find = 0;
    for (auto &it : loadedFonts) {
        if (it.filename == filename and it.size == size) {
            find = &it;
        }
    }
    if (find) {
        _data->font = find->font;
    }
    else {
        _data->font = TTF_OpenFont(filename.c_str(), size);
        if (_data->font) {
            loadedFonts.push_back({filename, size, _data->font});
        }
    }
#endif
}

Font::Font() : _data(new FontData) {
    if (!isInitialized) {
#ifndef USE_BITMAP_FONT
        TTF_Init();
#endif
        isInitialized = true;
    }
}

Font::Font(int size) : Font(defaultFontPath, size) {
}

Font::Font(const Font &font) : _data(new FontData) {
    _data->font = font._data->font;
}

Font::~Font() {
}

void Font::draw(double x, double y, const std::string &text, bool centered) {
    renderText(_data->font, 255, 255, 255, x, y, 0, text, centered);
}

void Font::DefaultFontPath(std::string path) {
    defaultFontPath = path;
}

void Font::operator=(const Font &font) {
    _data->font = font._data->font;
}

Font::operator bool() const {
    return bool(_data->font);
}

} // namespace MatGui
