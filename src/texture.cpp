/*
 * texture.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/texture.h"

#include "matgui/common-gl.h"
//#include "SDL2/SDL_opengl.h"
#ifndef DISABLE_TEXTURES
#include "SDL2/SDL_image.h"
#endif // DISABLE_TEXTURES
#include "matgui/draw.h"
#include <map>

namespace MatGui {

static std::map<std::string, Texture> loadedTextures;

static bool isInitialized = false;


static unsigned int createTextureFromFile(const std::string filename) {
#ifdef DISABLE_TEXTURES
	return 0;
#else

	GLuint textureId = 0;

	if (not isInitialized) {
		IMG_Init(IMG_INIT_PNG);
	}

	std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)> surface(IMG_Load(filename.c_str()), SDL_FreeSurface);

	if (surface == NULL) {
		debug_print("could not load file %s\n", filename.c_str());
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


static unsigned int createGrayscaleFromPixels(const std::vector<float> &intensities, int width, int height) {
#ifdef DISABLE_TEXTURES
	return 0;
#else
	GLuint textureId = 0;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int mode = GL_RGBA;

	std::vector<Texture::Pixel> pixels;
	pixels.reserve(intensities.size());
	for (size_t i = 0; i < intensities.size(); ++i) {
		auto intensity = (unsigned char) 255 * intensities[i];
		pixels.push_back(Texture::Pixel(intensity, intensity, intensity));
	}

	glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, &pixels[0].r);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureId;
#endif
}

void Texture::interpolation(Interpolation interpolation) {
#ifndef DISABLE_TEXTURES
	glBindTexture(GL_TEXTURE_2D, *this);
	switch(interpolation) {
	case Interpolation::Linear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;

	case Interpolation::Nearest:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
#endif
}


void Texture::createBitmap(const std::vector<Pixel> &pixels, int width, int height, std::string name) {
	if (!name.empty()) {
		auto find = loadedTextures.find(name);
		if (find == loadedTextures.end()){
			texture(createTextureFromPixels(pixels, width, height));
			if (_texturePtr) {
				loadedTextures[name] = *this;
			}
		}
		else {
			*this = find->second;
		}
	}
	else {
		texture(createTextureFromPixels(pixels, width, height));
	}
}

void Texture::createGrayscale(const std::vector<float> &pixels, int width, int height, std::string name) {
	if (!name.empty()) {
		auto find = loadedTextures.find(name);
		if (find == loadedTextures.end()){
			texture(createGrayscaleFromPixels(pixels, width, height));
			if (_texturePtr) {
				loadedTextures[name] = *this;
			}
		}
		else {
			*this = find->second;
		}
	}
	else {
		texture(createGrayscaleFromPixels(pixels, width, height));
	}
}

Texture::Texture() {
}

Texture::~Texture() {
}


Texture::Texture(const std::string filename, bool addToLibrary) {
	load(filename, addToLibrary);
}


void Texture::texture(unsigned int ptr) {
	clear();
	_texturePtr = std::shared_ptr<void>((void *)(intptr_t) ptr, [](void *ptr) {
#ifndef DISABLE_TEXTURES
		glDeleteTextures(1, (unsigned int*) &ptr);
#endif
	});
}


void Texture::load(const std::string filename, bool addToLibrary) {
	auto find = loadedTextures.find(filename);
	if (find == loadedTextures.end()){
		texture(createTextureFromFile(filename));
		if (_texturePtr && addToLibrary) {
			loadedTextures[filename] = *this;
		}
	}
	else {
		*this = find->second;
	}
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, *this);
}
} /* namespace MatGui */

