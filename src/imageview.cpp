/*
 * imageview.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/imageview.h"
#include "matgui/draw.h"

namespace MatGui {

ImageView::ImageView() {
}

ImageView::~ImageView() {
}

void ImageView::draw() {
	drawTextureRect({_x, _y}, 0, _width, _height, _texture);
}

void ImageView::loadImage(std::string filename) {
	_texture.load(filename);

}

} /* namespace MatGui */
