#include "matgui/bitmapfont.h"
#include "matgui/keys.h"

#include <list>
#include <map>
#include <string>

//#define NDEBUG //Uncomment for production version
#include <assert.h>

using std::list;
using std::string;

namespace matgui {

// Forward declaration for readability
extern std::map<char, BitmapFontData> fontBitmap;
extern std::map<std::string, BitmapFontData> multiCharacterFontBitmap;

// static void printToConsole(const BitmapFontData & data) {
//	auto x = 0;
//	for (auto c: data) {
//		cout << c;
//		++x;
//		if (x >= data.width) {
//			cout << "|" << endl;
//			x = 0;
//		}
//	}
//	cout << "<--" << endl;
//}

BitmapFontData getFontDataVector(string text) {
    int width = 0;
    int len = 0;
    int textSize = text.size();
    string letter;
    letter.reserve(6);
    std::list<BitmapFontData *> letterList;

    unsigned lineHeight = 1;
    int lineDepth = 0;

    for (int i = 0; i < textSize; ++i) {
        auto c = text[i];
        BitmapFontData *letterData = 0;
        char nc = 0;
        if (i < textSize - 1) {
            nc = text[i + 1];
        }
        letter.clear();
        letter.push_back(c);
        if (isUtfTail(nc)) {
            while (isUtfTail(nc)) {
                letter.push_back(nc);
                ++i;
                if (i < textSize - 1) {
                    nc = text[i + 1];
                }
                else {
                    break;
                }
            }
            // Todo: This will probably crash on 3 char utf letters
            letterData = &multiCharacterFontBitmap[letter];
        }
        else {
            letterData = &fontBitmap[c];
        }

        if (letterData) {
            width += letterData->width;
            if (letterData->lineDepth > lineDepth) {
                lineDepth = letterData->lineDepth;
            }

            if (letterData->lineHeight > lineHeight) {
                lineHeight = letterData->lineHeight;
            }
            letterList.push_back(letterData);
            letterData = 0;
        }
    }

    int height = lineHeight + lineDepth;

    BitmapFontData fontData;
    fontData.height = height;
    fontData.width = width;
    len = fontData.width * fontData.height;
    fontData.resize(len);
    fontData.fill(' ');

    int rasterX = 0;
    int offsetY = 0;
    for (auto letter : letterList) {
        offsetY = lineHeight - letter->lineHeight;
        for (unsigned y = 0; y < letter->height; ++y) {
            for (unsigned x = 0; x < letter->width; ++x) {
                assert(x + y * width + rasterX < fontData.size());
                assert(x + y * letter->width < letter->size());

                char c = letter->get(x, y);
                fontData.get(x + rasterX, y + offsetY) = c;
            }
        }

        rasterX += letter->width; // Todo: something is wrong with this code
    }

    //	dout << "preview:" << endl;
    //	printToConsole(fontData);

    return fontData;
}

} // namespace matgui
