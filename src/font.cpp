/*
 * Font.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "font.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <list>
#include "shaderprogram.h"
#include "draw.h"

namespace MatGui {

struct FontDescriptionStruct {
	std::string filename;
	int size;
	TTF_Font *font;
};

struct FontData {
	TTF_Font *font = nullptr;
};


//Unoptimized function to draw text, consider to use fontview instead to buffer data
void renderText(const TTF_Font *font, GLubyte r, GLubyte g, GLubyte b,
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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (view._needsUpdate) {
			/*Create some variables.*/
			auto message = TTF_RenderUTF8_Blended(const_cast<TTF_Font*>(view._font._data->font), view._text.c_str(), color);

			if (texture == 0) {
				/*Generate an OpenGL 2D texture from the SDL_Surface*.*/
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, texture); //the texture name is already created
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, message->w, message->h, 0, GL_BGRA,
					GL_UNSIGNED_BYTE, message->pixels);

			view._width = message->w;
			view._height = message->h;
			SDL_FreeSurface(message);

			view._needsUpdate = false;
		}

		drawTextureRect({x, y}, 0, view._width, view._height, texture, centered ? DrawStyle::CenterOrigo : DrawStyle::OrigoTopLeft);
	}

	unsigned texture = 0;
	SDL_Color color = {255, 255, 255};
	bool centered = true;
};



static std::list<FontDescriptionStruct> loadedFonts;
static bool isInitialized = false;
static std::string defaultFontPath = "font/Ubuntu-R.ttf";




void renderText(const TTF_Font *font, GLubyte r, GLubyte g, GLubyte b,
                double x, double y, double z, const std::string& text, bool centered)
{
	if (!font) return;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*Create some variables.*/
	SDL_Color color = {r, g, b};
	SDL_Surface *message = TTF_RenderUTF8_Blended(const_cast<TTF_Font*>(font), text.c_str(), color);

	unsigned Texture = 0;

	/*Generate an OpenGL 2D texture from the SDL_Surface*.*/
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, message->w, message->h, 0, GL_BGRA,
	             GL_UNSIGNED_BYTE, message->pixels);

	drawTextureRect({x, y}, 0, message->w, message->h, Texture, centered ? DrawStyle::CenterOrigo : DrawStyle::OrigoTopLeft);

	/*Clean up.*/
	glDeleteTextures(1, &Texture);
	SDL_FreeSurface(message);

}



Font::Font(const std::string filename, int size): Font() {
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
}

Font::Font(): _data(new FontData) {
	if (!isInitialized) {
		TTF_Init();
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

