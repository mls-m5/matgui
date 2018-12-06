/*
 * imageview.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <string>

#include "texture.h"
#include "view.h"

namespace MatGui {

class ImageView : public View {
public:
	ImageView();
	ImageView(std::string filename) {
		loadImage(filename);
	}
	ImageView(Texture texture):
		_texture(texture)
		{}
	virtual ~ImageView();

	void draw() override;

	void loadImage(std::string filename);

	Texture _texture;
};

} /* namespace MatGui */
