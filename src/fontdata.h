#pragma once

#ifdef USE_BITMAP_FONT
#include "matgui/bitmapfont.h"

typedef void FontType;

#else

#include <SDL2/SDL_ttf.h>

typedef TTF_Font FontType;

#endif

namespace matgui {

class FontData {
public:
    FontType *font = nullptr;
};

} // namespace matgui
