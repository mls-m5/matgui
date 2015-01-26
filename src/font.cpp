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

struct fontDescriptionStruct {
	std::string filename;
	int size;
	TTF_Font *font;
};

struct FontData {
	TTF_Font *font;
};

static std::list<fontDescriptionStruct> loadedFonts;
static bool isInitialized = false;


void RenderText(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B,
                const double& X, const double& Y, const double& Z,  const std::string& Text, bool centered)
{
	if (!Font) return;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*Create some variables.*/
	SDL_Color Color = {R, G, B};
	SDL_Surface *Message = TTF_RenderUTF8_Blended(const_cast<TTF_Font*>(Font), Text.c_str(), Color);
	unsigned Texture = 0;

	/*Generate an OpenGL 2D texture from the SDL_Surface*.*/
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA,
	             GL_UNSIGNED_BYTE, Message->pixels);

	drawTextureRect({X, Y}, 0, Message->w, Message->h, Texture, centered ? DrawStyle::CenterOrigo : DrawStyle::OrigoTopLeft);

	/*Clean up.*/
	glDeleteTextures(1, &Texture);
	SDL_FreeSurface(Message);

}



Font::Font(const std::string filename, int size) {
	data = new FontData;

	fontDescriptionStruct *find = 0;
	for (auto &it: loadedFonts) {
		if (it.filename == filename and it.size == size) {
			find = &it;
		}
	}
	if (find) {
		data->font = find->font;
	}
	else {
		if (!isInitialized) {
			TTF_Init();
			isInitialized = true;
		}
		data->font = TTF_OpenFont(filename.c_str(), size);
		if (data->font) {
			loadedFonts.push_back({filename, size, data->font});
		}
	}
}

Font::~Font() {
	delete data;
}

void Font::draw(double x, double y, const std::string& text, bool centered) {
	RenderText(data->font, 255, 255, 255, x, y, 0, text, centered);
}

} /* namespace MatGui */
