/*
 * texture.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

namespace MatGui {

// The texture class is supposed to be used by value
// Because the textureId is kept in a shared_ptr the background texture is freed
// when needed
class Texture {
public:
	enum Interpolation {
		Linear = 0,
		Nearest = 1
	};

	struct Pixel {
		unsigned char r=0,g=0,b=0,a=255;
		Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255):
			r(r), g(g), b(b), a(a) {}
		Pixel(unsigned int c, unsigned char a):
			Pixel(c & 0xFF, (c >> 8) & 0xFF, (c >>  8*2) & 0xFF, a) {}
		Pixel(unsigned int c):
			Pixel(c & 0xFF, (c >> 8) & 0xFF, (c >>  8*2) & 0xFF) {}
		Pixel() = default;
	};

	Texture();
	Texture(const std::string filename, bool addToLibrary = true);
	Texture(const Texture & texture) = default;
	Texture(Texture && texture) = default;

	virtual ~Texture();

	void load(const std::string filename, bool addToLibrary = true);
	void createBitmap(const std::vector<Pixel> &pixels, int width = 1, int height = 1, std::string name="");
	void createGrayscale(const std::vector<float> &grayScale, int width = 1, int height = 1, std::string name="");
	void clear() {
		_texturePtr = nullptr;
	}
	void bind();

	unsigned int texture() const {
		return (unsigned long)_texturePtr.get();
	}

	//Transfer ownership of raw reference to this object
	//the reference is handled by the texture object
	void texture(unsigned int texture);

	operator unsigned int() const {
		return texture();
	}

	bool unique() const {
		return _texturePtr.unique();
	}

	Texture &operator =(const Texture &) = default;

	[[deprecated]] void setInterpolation(Interpolation i) {
		interpolation(i);
	}

	void interpolation(Interpolation);

	std::shared_ptr<void> _texturePtr;
};

} /* namespace MatGui */
