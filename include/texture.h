/*
 * texture.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <string>

namespace MatGui {

class Texture {
public:
	Texture();
	Texture(const std::string filename);
	virtual ~Texture();

	void render();

	void load(const std::string filename);

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
