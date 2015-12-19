#include "bitmapfont.h"

#include <map>
#include <string>
#include <list>



//#define NDEBUG //Uncomment for production version
#include <assert.h>

using std::string;
using std::list;

 //Forward declaration for readability
extern std::map<char, BitmapFontData> fontBitmap;
extern std::map<std::string, BitmapFontData> multiCharacterFontBitmap;

bool isUtfTail(char c) {
	return((c & 0xC0) == 0x80);
}

//static void printToConsole(const BitmapFontData & data) {
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
	std::list<BitmapFontData*> letterList;

	int lineHeight = 1;
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
			//Todo: This will probably crash on 3 char utf letters
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
	for (auto letter: letterList) {
		offsetY = lineHeight - letter->lineHeight;
		for (int y = 0; y < letter->height; ++y) {
			for (int x = 0; x < letter->width; ++x) {
				assert(x + y * width + rasterX < fontData.size());
				assert(x + y * letter->width < letter->size());


				char c = letter->get(x, y);
				fontData.get(x + rasterX, y + offsetY) = c;
			}
		}

		rasterX += letter->width; //Todo: something is wrong with this code
	}

//	dout << "preview:" << endl;
//	printToConsole(fontData);

	return fontData;
}

std::map<char, BitmapFontData> fontBitmap = {
{ //Space
' ',
"l  \n",


},{

'1',

R"tag(
  x
 xx
  x
  x
  x
  x
lxxx
)tag",

},{
'2',
R"tag(
  xx
 x  x
    x
   x
  x
 x
lxxxx
)tag",
},{

'2',
R"tag(
 xxxx
    x
   x
    x
    x
 x  x
l xx
)tag",
},{


'4',
R"tag(
 x  x
 x  x
 xxxx
    x
    x
    x
l   x
)tag",
},{


'5',
R"tag(
 xxxx
 x
 xxx
    x
    x
 x  x
l xx
)tag",
},{



'6',
R"tag(
  xx
 x
 xxx
 x  x
 x  x
 x  x
l xx
)tag",
},{


'7',
R"tag(
 xxxx
    x
   x
  x
  x
  x
l x
)tag",
},{


'7',
R"tag(
  xx
 x  x
  xx
 x  x
 x  x
 x  x
l xx
)tag",
},{

'9',
R"tag(
  xx
 x  x
 x  x
  xxx
    x
    x
l xx
)tag",
},{

'.',
R"tag(
lx
)tag",
},{


':',
R"tag(
 x
 
 
lx
)tag",
},{


',',
R"tag(
lx
 x
)tag",
},{


';',
R"tag(
 x
 
 
 
lx
 x
)tag",
},{

'a',
R"tag(
  xx
 x  x
 x  x
l xxx
)tag",
},{


'A',
R"tag(
  xx
 x  x
 x  x
 xxxx
 x  x
lx  x
)tag",
},{


'b',
R"tag(
 x
 x
 xxx
 x  x
 x  x
lxxx
)tag",
},{

'B',
R"tag(
 xx
 x x
 xxx
 x  x
 x  x
lxxx
)tag",
},{



'c',
R"tag(
  xx
 x  
 x  x
l xx
)tag",
},{

'C',
R"tag(
  xx
 x  x
 x
 x  
 x  x
l xx
)tag",
},{



'd',
R"tag(
    x
    x
  xxx
 x  x
 x  x
l xxx
)tag",
},{

'D',
R"tag(
 xxx
 x  x
 x  x
 x  x
 x  x
lxxx
)tag",
},{



'e',
R"tag(
  xxx
 xxxxx
 x  
l xxx
)tag",
},{

'E',
R"tag(
 xxxx
 x
 x 
 xxx
 x  
lxxxx
)tag",
},{


'f',
R"tag(
  xxx
 x
 xxx
 x
 x  
lx
)tag",
},{

'F',
R"tag(
 xxxx
 x
 x 
 xxx
 x  
lx
)tag",
},{



'g',
R"tag(
  xxx
 x  x
 x  x
l xxx
    x
  xx
)tag",
},{

'G',
R"tag(
  xx
 x  x
 x 
 x xx
 x  x
l xx
)tag",
},{



'h',
R"tag(
 x
 x
 xxx
 x  x
lx  x
)tag",
},{

'H',
R"tag(
 x  x
 x  x
 xxxx
 x  x
 x  x
lx  x
)tag",
},{



'i',
R"tag(
 x
 
 x
 x
 x
lx
)tag",
},{

'I',
R"tag(
 xxx
  x
  x
  x
  x
lxxx 
)tag",
},{


'j',
R"tag(
 x
 
 x
 x
 x
lx
 x
x 
)tag",
},{

'J',
R"tag(
  xxx
    x
    x
    x
 x  x
l xx 
)tag",
},{

'k',
R"tag(
 x
 x  x
 x x
 xx 
 x x
lx  x
)tag",
},{

'K',
R"tag(
 x  x
 x x
 xx
 x x
 x  x
lx   x
)tag",
},{


'l',
R"tag(
 xx
  x
  x
  x
  x
lxxx
)tag",
},{

'L',
R"tag(
 xx
  x
  x
  x
  x   x
lxxxxxx 
)tag",
},{



'm',
R"tag(
 xx x 
 x x x
 x x x
lx   x
)tag",
},{

'M',
R"tag(
 x     x
 xx   xx
 x x x x
 x  x  x
 x     x
lxx   xx
)tag",
},{


'n',
R"tag(
 xxxx 
 x   x
 x   x
lx   x
)tag",
},{

'N',
R"tag(
 x   xx
 xx   x
 x x  x
 x  x x
 x   xx
lxx   x
)tag",
},{



'o',
R"tag(
  xxx 
 x   x
 x   x
l xxx
)tag",
},{

'o',
R"tag(
  xxx
 x   x
 x   x
 x   x
 x   x
l xxx
)tag",
},{



'p',
R"tag(
 xxx 
 x  x
 x  x
lxxx
 x
 x
)tag",
},{

'P',
R"tag(
 xxxx
 x   x
 x   x
 xxxx
 x
lx
)tag",
},{


'q',
R"tag(
  xxxx
 x   x
 x   x
l xxxx
     x
     x
)tag",
},{

'o',
R"tag(
  xxx
 x   x
 x   x
 x   x
 x  xx
l xxx x
)tag",
},{



'r',
R"tag(
 xxx 
 x  x
 x
lx
)tag",
},{

'R',
R"tag(
 xxxx
 x   x
 x   x
 xxxx
 x  x
lx   x
)tag",
},{



's',
R"tag(
  xx 
 x  
  xx
    x
lxxx
)tag",
},{

'S',
R"tag(
  xxx
 x   x
 x   
  xxx
     x
 x   x
l xxx
)tag",
},{




't',
R"tag(
 xx
  x
  x
 xxx 
  x
  x
lxxx
)tag",
},{

'T',
R"tag(
 xxxxx
 x x x
   x   
   x
   x
   x
l xxx
)tag",
},{



'u',
R"tag(
 x   x 
 x   x
 x   x
l xxxx
)tag",
},{

'U',
R"tag(
 x    x
 x    x
 x    x
 x    x
 x    x
l xxxxx
)tag",
},{



'v',
R"tag(
 x   x 
 x   x
  x x
l  x
)tag",
},{

'V',
R"tag(
 x   x
 x   x
 x   x
 x   x
  x x
l  x
)tag",
},{

'x',
R"tag(
 x   x 
   x
  x x
lx   x
)tag",
},{

'X',
R"tag(
 x   x
  x x
   x
  x x
 x   x
lx   x
)tag",
},{



'y',
R"tag(
 x  x 
 x  x
 x  x
l xxx
    x
    x
   x
)tag",
},{

'Y',
R"tag(
 x   x
 x   x
  x x
   x
   x
   x
l  x
)tag",
},{


'z',
R"tag(
 xxxx 
   x
  x
lxxxx
)tag",
},{

'z',
R"tag(
 xxxxx
 x   x
    x
   x
  x
 x   x
lxxxxx
)tag",
}
};


std::map<std::string, BitmapFontData> multiCharacterFontBitmap = {
{

"ä",
R"tag(
 x x

  xx
 x  x
 x  x
l xxx
)tag",
},{


"Ä",
R"tag(
 x  x

  xx
 x  x
 x  x
 xxxx
 x  x
lx  x
)tag",
},{



"å",
R"tag(
  xx

  xx
 x  x
 x  x
l xxx
)tag",
},{


"Å",
R"tag(
  xx

  xx
 x  x
 x  x
 xxxx
 x  x
lx  x
)tag",
},{


"ö",
R"tag(
  x x
 
  xxx 
 x   x
 x   x
l xxx
)tag",
},{

"Ö",
R"tag(
  x x 

  xxx
 x   x
 x   x
 x   x
 x   x
l xxx
)tag",
}

};

