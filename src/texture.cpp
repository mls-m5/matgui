/*
 * texture.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "texture.h"

#include "SDL2/SDL_opengl.h"
#include "SDL2/SDL_image.h"
#include "draw.h"
#include <map>

namespace MatGui {

static std::map<std::string, unsigned int> loadedTextures;

static bool isInitialized = false;


unsigned int createTextureFromFile(const std::string filename) {

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
}


Texture::Texture() {
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	unsigned char pixels[_width * _height * 3];
	for (int x = 0; x < _width; ++x) {
		for (int y = 0; y < _height; ++y) {
			for (int c = 0; c < 3; ++c) {
				pixels[(x + y * _width) * 3 + c] = 100 * x / _width + ((y % 10) > 5? 0 : 100);
			}
		}
	}

	glTexImage2D(
			GL_TEXTURE_2D,         // target
			0,                     // level, 0 = base, no minimap,
			GL_RGB,                // internalformat
			_width,     // width
			_height,    // height
			0,                     // border, always 0 in OpenGL ES
			GL_RGB,                // format
			GL_UNSIGNED_BYTE,      // type
			pixels
	);

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
