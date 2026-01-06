// Copyright © Mattias Larsson Sköld 2020

#include "../src/translateshader.cpp"
#include "mls-unit-test/unittest.h"
#include <sstream>
#include <string>

namespace {
std::string lastLine(std::string code) {
    std::stringstream ss(code);

    std::string line;
    std::string ret;

    while (getline(ss, line)) {
        if (!line.empty()) {
            ret = line;
        }
    }

    return ret;
}
} // namespace

TEST_SUIT_BEGIN(MatguiShaderTranslate)

TEST_CASE("frag color/out") {
    constexpr auto code = //
        "#version 330\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "   fragColor = vec4(1, 1, 1);\n"
        "}";
    auto res = translateShader(code, GL_FRAGMENT_SHADER);
    std::cout << res;
}

TEST_CASE("disabled frag in") {
    constexpr auto code = " in apa;";

    auto res = lastLine(translateShader(code, GL_FRAGMENT_SHADER));
    ASSERT_EQ(res, "varying apa;");
}

TEST_CASE("disabled vertex out") {
    constexpr auto code = " out apa;";

    auto res = lastLine(translateShader(code, GL_VERTEX_SHADER));

    ASSERT_EQ(res, "varying apa;");
}

TEST_CASE("disabled vertex in") {
    constexpr auto code = " in apa;";

    auto res = lastLine(translateShader(code, GL_VERTEX_SHADER));

    ASSERT_EQ(res, "attribute apa;");
}

TEST_CASE("version") {
    constexpr auto code = "#version 330";

    auto res = lastLine(translateShader(code, GL_VERTEX_SHADER));

    ASSERT_NE(res, "#version 330");
}

TEST_CASE("remove layout") {
    constexpr auto code = "layout (location = 0) in vec4 aPosition;";

    const auto res = lastLine(translateShader(code, GL_VERTEX_SHADER));

    std::cout << res << std::endl;

    ASSERT_NE(res, code);
}

TEST_CASE("texture2d --> texture") {
    constexpr auto code = "texture2D texture2D";

    const auto res = lastLine(translateShader(code, GL_FRAGMENT_SHADER));
}

TEST_SUIT_END
