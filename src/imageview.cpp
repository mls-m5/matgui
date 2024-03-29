/*
 * imageview.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/imageview.h"
#include "matgui/draw.h"

namespace matgui {

ImageView::ImageView() = default;

ImageView::~ImageView() = default;

void ImageView::draw() {
    drawTextureRect(
        {x(), y()}, 0, width(), height(), _texture, DrawStyle::OrigoTopLeft);
}

void ImageView::loadImage(std::string filename) {
    _texture.load(filename);
}

} /* namespace matgui */
