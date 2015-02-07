/*
 * Font.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <string>

namespace MatGui {

class Font {
public:
	Font(const std::string, int size);
	Font(int size);
	virtual ~Font();

	void draw(double x, double y, const std::string &text, bool centered);

	static void setDefaultFontPath(std::string path);

	class FontData* data;
};

} /* namespace MatGui */
