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

namespace matgui {

class ImageView : public View {
public:
    ImageView();
    ImageView(std::string filename) {
        loadImage(filename);
    }
    ImageView(const Texture &texture) : _texture(texture) {
    }
    virtual ~ImageView();

    void draw() override;

    void loadImage(std::string filename);
    void texture(const Texture &texture) {
        _texture = texture;
    }
    Texture &texture() {
        return _texture;
    };

    Texture _texture;
};

} /* namespace matgui */
