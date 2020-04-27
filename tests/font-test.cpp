/*
 * font-test.cpp
 *
 *  Created on: 21 jun 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/bitmapfont.h"
#include "mls-unit-test/unittest.h"
#include <iostream>

using namespace std;
using namespace MatGui;

namespace {

BitmapFontData apData("xx\nxx\n");
BitmapFontData bepData("xx\nxx\n");

} // namespace

TEST_SUIT_BEGIN

TEST_CASE("concat chars") {
    auto a = getFontDataVector("a");
    auto b = getFontDataVector("b");
    auto ab = getFontDataVector("ab");

    ASSERT_GT(a.width, 0);
    ASSERT_GT(b.width, 0);
    ASSERT_GT(ab.width, 0);
    ASSERT_EQ(a.width + b.width, ab.width);

    ASSERT_GT(a.height, 1);

    auto abcd = getFontDataVector("abcdefg");
}

TEST_CASE("utf-8 chars") {
    auto ae = getFontDataVector("ö");
}

TEST_SUIT_END
