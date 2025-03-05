
cmake_minimum_required(VERSION 3.22)

enable_testing()


add_executable(
    matgui-tests
    font-test.cpp
    shadertranslate-test.cpp
    signal-test.cpp
    mls-unit-test/test_main.cpp
)

target_include_directories(
    matgui-tests
    PRIVATE
    tests
)

add_test(
    NAME matgui-tests
    COMMAND matgui-tests
)

target_link_libraries(
    matgui-tests
    MatGui
    SDL2_image
)


