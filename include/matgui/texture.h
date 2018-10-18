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

	};

	Texture();
	Texture(const std::string filename);
	virtual ~Texture();

	void render();

	void load(const std::string filename);
	void createBitmap(const std::vector<Pixel> &pixels, int width = 1, int height = 1, std::string name="");

	unsigned int texture() {
		return _textureId;
	}

	operator unsigned int() {
		return texture();
	}

	unsigned int _textureId = 0;
	int _width = 100; //test values
	int _height = 100;

};

} /* namespace MatGui */
