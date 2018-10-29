/*
 * texture.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <string>
#include <vector>

namespace MatGui {

class Texture {
public:
	struct Pixel {
		unsigned char r=0,g=0,b=0,a=255;
		Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255):
			r(r), g(g), b(b), a(a) {}
		Pixel() = default;
	};

	Texture();
	Texture(const std::string filename);
	virtual ~Texture();

	void render();

	void load(const std::string filename);
	void createBitmap(const std::vector<Pixel> &pixels, int width = 1, int height = 1, std::string name="");
	void clear();

	unsigned int texture() {
		return _textureId;
	}

	void texture(unsigned int textureId, bool unique = true) {
		clear();
		_textureId = textureId;
		_unique = unique;
	}

	operator unsigned int() {
		return texture();
	}

	bool unique() {
		return _unique;
	}

	unsigned int _textureId = 0;
	bool _unique = false; //If the texture is to be deleted when the destructor is called
};

} /* namespace MatGui */
