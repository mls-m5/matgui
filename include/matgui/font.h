/*
 * Font.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <string>
#include <memory>

namespace MatGui {

class Font {
public:
	Font();
	Font(const std::string, int size);
	Font(int size);
	Font(const Font &font);
	virtual ~Font();

	void draw(double x, double y, const std::string &text, bool centered);

	void operator=(const Font&);

	static void setDefaultFontPath(std::string path);

	operator bool();

protected:
	std::unique_ptr<class FontData> _data;

	friend class FontView;
	friend class FontViewData;
};

class FontView {
public:
	FontView();
	~FontView();

	void font(const Font &font) {
		_font = font;
		_needsUpdate = true;
	}

	const Font& font() {
		return _font;
	}

	int width() {
		return _width;
	}

	int height() {
		return _height;
	}

	void text(std::string text) {
		if (_text != text) {
			_text = text;
			_needsUpdate = true;
		}
	}

	const std::string &text() {
		return _text;
	}

	operator bool() {
		return bool(_font);
	}

	void draw(double x, double y);

protected:
	int _width = 0;
	int _height = 0;
	Font _font;
	std::string _text;
	bool _needsUpdate = false;

	std::unique_ptr<class FontViewData> _data;

	friend class FontViewData;
};



} /* namespace MatGui */
