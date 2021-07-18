
#pragma once

#include <ciso646> //For or, and, etc...
#include <string>
#include <vector>

namespace matgui {

struct ColorType {
    unsigned char r, g, b, a;
};

struct BitmapFontData getFontDataVector(std::string text);

struct BitmapFontData : public std::vector<char> {
    BitmapFontData() {
    }

    BitmapFontData(const char *data);

    void setDimensions(int w, int h) {
        width = w;
        height = h;
        resize(width * height);
    }

    char &get(int x, int y) {
        return this->at(x + y * width);
    }

    inline void fill(char value = ' ') {
        std::fill(begin(), end(), value);
    }

    unsigned width = 0;
    unsigned height = 0;
    unsigned lineHeight = 0; // Line distance from top
    int lineDepth = 0;       // Line distance from bottom
    int linePos = 0;
};

struct BitmapFont {
    int w = 0, h = 0, lineHeight = 0;

    std::vector<unsigned char> pixels;
    ColorType *colors() {
        return reinterpret_cast<ColorType *>(pixels.data());
    }

    BitmapFont(){};

    BitmapFont(BitmapFontData *data) {
        setContent(*data);
    }

    BitmapFont(const std::string &text) {
        auto data = getFontDataVector(text);
        setContent(data);
    }

    ~BitmapFont() {
        clear();
    }

    void clear() {
        pixels.clear();
    }

    void create(int width, int height) {
        clear();
        w = width;
        h = height;
        pixels.resize(width * height * 4);
    }

    void setContent(const BitmapFontData &fdata) {
        auto width = fdata.width;
        auto height = fdata.height;
        auto data = fdata.data();
        lineHeight = fdata.lineHeight;
        if (width != w or height != h) {
            create(width, height);
        }

        int len = w * h;
        for (int i = 0; i < len; ++i) {
            switch (data[i]) {
            case 'x':
                colors()[i] = {255, 255, 255, 255};
                break;
            case ' ':
                colors()[i] = {0, 0, 0, 0};
                break;
            }
        }
    }
};

inline BitmapFontData::BitmapFontData(const char *data) {
    unsigned h = 1;
    unsigned w = 0;
    // get the dimensions of the letter
    for (int i = 0; data[i] != 0; ++i) {
        auto &c = data[i];
        if (c == '\n') {
            ++h;
            if (w > width) {
                width = w;
            }
            w = 0;
        }
        ++w;
    }

    height = h;
    resize(width * height);
    fill(' ');

    // Copy the data to right place
    int x = 0;
    int y = 0;
    for (int i = 0; data[i] != 0; ++i) {
        auto c = data[i];
        if (c == '\n') {
            x = 0;
            ++y;
        }
        else if (c == 'l') {
            get(x, y) = ' ';
            linePos = y;
            ++x;
        }
        else {
            get(x, y) = c;
            ++x;
        }
    }
    lineHeight = linePos;
    lineDepth = height - linePos;
}

} // namespace matgui
