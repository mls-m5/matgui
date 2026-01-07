#pragma once

#include "texture.h"
#include <filesystem>

namespace matgui {

class NineSlice {
public:
    NineSlice();

    void loadTexture(const std::filesystem::path path);
    void border(float width);

    const matgui::Texture &texture() const;
    float border() const;

    void draw(); // ..how

private:
    float _border = 1;

    matgui::Texture _texture;
};

} // namespace matgui
