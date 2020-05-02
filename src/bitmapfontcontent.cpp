/*
 * bitmapfontdata.cpp
 *
 *  Created on: 9 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/bitmapfont.h"

#include <map>

namespace MatGui {

// clang-format off
std::map<char, BitmapFontData> fontBitmap = {
{ //Space
' ',
"l  \n",


},{
'0',

R"tag(
  
  xx
 x  x
 x  x
 x  x
 x  x
l xx
)tag",

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

'3',
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


'8',
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
l xx
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



'w',
R"tag(
 x       x
 x   x   x
  x x x x
l  x   x
)tag",
},{

'W',
R"tag(
 x       x
 x       x
 x   x   x
 x   x   x
  x x x x
l  x   x
)tag",
},{


'x',
R"tag(
 x   x 
  x x 
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

'Z',
R"tag(
 xxxxx
 x   x
    x
   x
  x
 x   x
lxxxxx
)tag",
},

{
'-',
R"tag(
 xxx
  
 
l
)tag",
},

{
'+',
R"tag(
  x
  x
xxxxx
  x
  x
l
)tag",
},



{

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


'=',
R"tag(
 xxx
 
 
lxxx
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
},


{
'!',
R"tag(
 xx
 x
 x
 x
 x

lx
)tag",
},


{
'/',
R"tag(
   x
   x
  x
  x
 x
lx
)tag",
},


{
'|',
R"tag(
 x
 x
 x
 x
 x
lx
)tag",
},


{
'\\',
R"tag(
 x
 x
  x
  x
   x
l  x
)tag",
},


{
'?',
R"tag(
 xx
x  x
  x
  x
   
l x
)tag",
},




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

} // namespace MatGui
