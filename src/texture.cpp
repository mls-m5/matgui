/*
 * texture.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "texture.h"

#include "common-gl.h"
//#include "SDL2/SDL_opengl.h"
#ifndef DISABLE_TEXTURES
#include "SDL2/SDL_image.h"
#endif // DISABLE_TEXTURES
#include "draw.h"
#include <map>

namespace MatGui {

static std::map<std::string, unsigned int> loadedTextures;

static bool isInitialized = false;


unsigned int createTextureFromFile(const std::string filename) {
#ifdef DISABLE_TEXTURES
	return 0;
#else

	GLuint textureId = 0;

	if (not isInitialized) {
		IMG_Init(IMG_INIT_PNG);
	}
	SDL_Surface* surface = IMG_Load(filename.c_str());

	if (surface == NULL) {
		return 0;
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int mode = GL_RGB;

	if(surface->format->BytesPerPixel == 4) {
	    mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureId;
#endif
}


Texture::Texture() {
//	glGenTextures(1, &_textureId);
//	glBindTexture(GL_TEXTURE_2D, _textureId);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//


}

Texture::~Texture() {
	glDeleteTextures(1, &_textureId);
}

Texture::Texture(const std::string filename) {
	load(filename);
}

void Texture::load(const std::string filename) {
	auto find = loadedTextures.find(filename);
	if (find == loadedTextures.end()){
		_textureId = createTextureFromFile(filename);
		if (_textureId) {
			loadedTextures[filename] = _textureId;
		}
	}
	else {
		_textureId = find->second;
	}
}

void Texture::render() {
	drawTextureRect({0,0}, 30, 100, 100, _textureId);
}

} /* namespace MatGui */

