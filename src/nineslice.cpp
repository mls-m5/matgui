#include "matgui/nineslice.h"

namespace matgui {

void NineSlice::setUniformBorderWidth(float width) {
    _borderWidth = width;
}

float NineSlice::border() const {
    return _border;
}

const Texture &NineSlice::texture() const {
    return _texture;
}

void NineSlice::loadTexture(const std::filesystem::path path) {
    _texture.load(path);
}

} // namespace matgui
