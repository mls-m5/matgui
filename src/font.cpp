/*
 * Font.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "font.h"

#ifdef __ANDROID__
#define USE_BITMAP_FONT //Define this variable to have bitmap fonts instead
#endif

#ifdef USE_BITMAP_FONT
#include "bitmapfont.h"

#include "common-gl.h"

typedef void FontType;
#else

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
typedef TTF_Font FontType;
typedef SDL_Color ColorType;
#endif

#include <SDL2/SDL_opengl.h>
#include <list>
#include "draw.h"


namespace MatGui {


struct FontDescriptionStruct {
	std::string filename;
	int size;
	FontType *font;
};

struct FontData {
	FontType *font = nullptr;
};


//Unoptimized function to draw text, consider to use fontview instead to buffer data
void renderText(const FontType *font, GLubyte r, GLubyte g, GLubyte b,
                double x, double y, double z, const std::string& text, bool centered);

struct FontViewData {
	~FontViewData () {
		if (texture) {
			glDeleteTextures(1, &texture);
		}
	}

	void render(FontView &view, double x, double y) {
		if (view._text.empty()) return;
		if (!view._font) return;

		if (view._needsUpdate) {
			/*Create some variables.*/
#ifdef USE_BITMAP_FONT
			auto messageSurface = new BitmapFont(view._text);

#else
			auto messageSurface = TTF_RenderUTF8_Blended(const_cast<TTF_Font*>(view._font._data->font), view._text.c_str(), color);
#endif

			if (texture == 0) {
				/*Generate an OpenGL 2D texture from the SDL_Surface*.*/
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
#ifdef USE_BITMAP_FONT
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#else
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif
			}
			else {
				glBindTexture(GL_TEXTURE_2D, texture); //the texture name is already created
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, messageSurface->w, messageSurface->h, 0, GL_BGRA,
					GL_UNSIGNED_BYTE, messageSurface->pixels);

			view._width = messageSurface->w;
			view._height = messageSurface->h;

#ifdef USE_BITMAP_FONT

#else
			SDL_FreeSurface(messageSurface);
#endif

			view._needsUpdate = false;
		}

		drawTextureRect({x, y}, 0, view._width, view._height, texture, centered ? DrawStyle::CenterOrigo : DrawStyle::OrigoTopLeft);
	}

	unsigned texture = 0;
	ColorType color = {255, 255, 255};
	bool centered = true;
};



static std::list<FontDescriptionStruct> loadedFonts;
static bool isInitialized = false;
static std::string defaultFontPath = "font/Ubuntu-R.ttf";




void renderText(const FontType *font, GLubyte r, GLubyte g, GLubyte b,
                double x, double y, double z, const std::string& text, bool centered)
{
	if (!font) return;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*Create some variables.*/
#ifdef USE_BITMAP_FONT
	auto data = getFontDataVector(text);
	auto messageSurface = new BitmapFont(text);
#else
	SDL_Color color = {r, g, b};
	SDL_Surface *messageSurface = TTF_RenderUTF8_Blended(const_cast<TTF_Font*>(font), text.c_str(), color);
#endif

	unsigned Texture = 0;

	/*Generate an OpenGL 2D texture from the SDL_Surface*.*/
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, messageSurface->w, messageSurface->h, 0, GL_BGRA,
	             GL_UNSIGNED_BYTE, messageSurface->pixels);

	drawTextureRect({x, y}, 0, messageSurface->w, messageSurface->h, Texture, centered ? DrawStyle::CenterOrigo : DrawStyle::OrigoTopLeft);

	/*Clean up.*/
	glDeleteTextures(1, &Texture);
#ifndef USE_BITMAP_FONT
	SDL_FreeSurface(messageSurface);
#endif

}



Font::Font(const std::string filename, int size): Font() {
#ifdef USE_BITMAP_FONT
	_data->font = (void*)-1; //some random non-null value
#else
	FontDescriptionStruct *find = 0;
	for (auto &it: loadedFonts) {
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

Font::Font(): _data(new FontData) {
	if (!isInitialized) {
#ifndef USE_BITMAP_FONT
		TTF_Init();
#endif
		isInitialized = true;
	}
}

Font::Font(int size): Font(defaultFontPath, size) {
}

Font::Font(const Font& font):
	_data(new FontData){
	_data->font = font._data->font;
}


Font::~Font() {
}

void Font::draw(double x, double y, const std::string& text, bool centered) {
	renderText(_data->font, 255, 255, 255, x, y, 0, text, centered);
}

void Font::setDefaultFontPath(std::string path) {
	defaultFontPath = path;
}

void Font::operator =(const Font& font) {
	_data->font = font._data->font;
}

Font::operator bool() {
	return bool(_data->font);
}

FontView::FontView(): _data(new FontViewData) {

}

FontView::~FontView() {
}

void FontView::draw(double x, double y) {
	//Todo implement
	_data->render(*this, x, y);
}

} /* namespace MatGui */

