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
	drawTextureRect({x(), y()}, 0, width(), height(), _texture);
}

void ImageView::loadImage(std::string filename) {
	_texture.load(filename);

}

} /* namespace MatGui */
