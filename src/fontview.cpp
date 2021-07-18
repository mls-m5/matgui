/*
 * fontview.cpp
 *
 *  Created on: 9 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/fontview.h"
#include "fontdata.h"
#include "matgui/common-gl.h"
#include "matgui/draw.h"

#ifdef USE_BITMAP_FONT
#include "matgui/bitmapfont.h"

#else

#include <SDL2/SDL_ttf.h>

typedef SDL_Color ColorType;
#endif

using namespace matgui;

namespace matgui {

class FontView::FontViewData {
public:
    ~FontViewData() {
        if (texture) {
            glDeleteTextures(1, &texture);
        }
    }

    void render(FontView &view, double x, double y) {
        if (view._text.empty())
            return;
        if (!view._font)
            return;

        if (view._needsUpdate) {
            // Create some variables.
#ifdef USE_BITMAP_FONT
            auto messageSurface = std::make_unique<BitmapFont>(view._text);
            auto pixels = [&] { return messageSurface->pixels.data(); };

            lineHeight = messageSurface->lineHeight;
            //            lineHeight = 0;

#else
            auto messageSurface = TTF_RenderUTF8_Blended(
                const_cast<TTF_Font *>(view._font._data->font),
                view._text.c_str(),
                color);
            auto pixels = [&] { return messageSurface->pixels; };
#endif

            if (texture == 0) {
                // Generate an OpenGL 2D texture from the SDL_Surface.
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE); // This is important for webgl otherwhise
                                       // only a black box is drawn
                glTexParameteri(GL_TEXTURE_2D,
                                GL_TEXTURE_WRAP_T,
                                GL_CLAMP_TO_EDGE); // for non pow2 size textures

#ifdef USE_BITMAP_FONT
                glTexParameteri(
                    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameterf(
                    GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#else
                glTexParameteri(
                    GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameterf(
                    GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif
            }
            else {
                glBindTexture(GL_TEXTURE_2D,
                              texture); // the texture name is already created
            }

            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         MATGUI_INTERNAL_TEXTURE_FORMAT,
                         messageSurface->w,
                         messageSurface->h,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         pixels());

            view._width = messageSurface->w;
            view._height = messageSurface->h;

#ifdef USE_BITMAP_FONT

#else
            SDL_FreeSurface(messageSurface);
#endif

            view._needsUpdate = false;
        }

        if (alignment) {
            x += static_cast<double>(view.width() * alignment) / 2.;
        }

        double scale = 1;

        drawTextureRect({x, y - lineHeight * scale},
                        0,
                        view._width * scale,
                        view._height * scale,
                        texture,
                        DrawStyle::OrigoTopLeft);
    }

    unsigned texture = 0;
    ColorType color = {255, 255, 255, 1};
    FontView::Alignment alignment = FontView::Center;
    int lineHeight = 0;
};

FontView::FontView() : _data(std::make_unique<FontViewData>()) {
}

matgui::FontView::FontView(std::string text) : FontView() {
    this->text(text);
}

FontView::FontView(const FontView &f)
    : _width(f._width), _height(f._height), _font(f._font), _text(f._text),
      _needsUpdate(true), _data(std::make_unique<FontViewData>()) {
}

FontView &FontView::operator=(const FontView &f) {
    _width = f._width;
    _height = f._height;
    _font = f._font;
    _text = f._text;
    _needsUpdate = true;
    return *this;
}

FontView &FontView::operator=(FontView &&) = default;

FontView::FontView(FontView &&) = default;

FontView::~FontView() {
}

void FontView::draw(double x, double y) {
    _data->render(*this, x, y);
}

void FontView::alignment(Alignment value) {
    _data->alignment = value;
}

matgui::FontView::Alignment FontView::alignment() {
    return _data->alignment;
}

} // namespace matgui
