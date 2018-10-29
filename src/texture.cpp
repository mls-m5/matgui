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

static unsigned int createTextureFromPixels(const std::vector<Texture::Pixel> &pixels, int width, int height) {
#ifdef DISABLE_TEXTURES
	return 0;
#else
	GLuint textureId = 0;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int mode = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, &pixels[0].r);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureId;
#endif
}

void Texture::createBitmap(const std::vector<Pixel> &pixels, int width, int height, std::string name) {
	if (!name.empty()) {
		auto find = loadedTextures.find(name);
		if (find == loadedTextures.end()){
			texture(createTextureFromPixels(pixels, width, height), false);
			if (_textureId) {
				loadedTextures[name] = _textureId;
			}
		}
		else {
			texture(find->second, false);
		}
	}
	else {
		texture(createTextureFromPixels(pixels, width, height), true);
	}
}

Texture::Texture() {
}

Texture::~Texture() {
	clear();
}

void Texture::clear() {
#ifndef DISABLE_TEXTURES
	if (_unique && _textureId) {
		glDeleteTextures(1, &_textureId);
		_textureId = 0;
	}
#endif
}

Texture::Texture(const std::string filename) {
	load(filename);
}

void Texture::load(const std::string filename) {
	auto find = loadedTextures.find(filename);
	if (find == loadedTextures.end()){
		texture(createTextureFromFile(filename), false);
		if (_textureId) {
			loadedTextures[filename] = _textureId;
		}
	}
	else {
		texture(find->second, false);
	}
}

void Texture::render() {
	drawTextureRect({0,0}, 30, 100, 100, _textureId);
}

} /* namespace MatGui */

