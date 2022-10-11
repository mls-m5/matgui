
cmake_minimum_required(VERSION 3.22)


project(MatGui)

find_package(SDL2 REQUIRED)

add_library(MatGui SHARED matgui.cpp)

target_include_directories(
    MatGui
    PUBLIC
       include
       ${SDL2_INCLUDE_DIRS}
       ${OPENGL_INCLUDE_DIR}
       SDL2_image
    PRIVATE
       src
    )

target_link_libraries(
    MatGui
    ${SDL2_LIBRARIES}
    )

if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
  message(" * C++ compiler: Emscripten")
  set(CMAKE_CXX_COMPILER_ID "Emscripten")
endif()

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
find_package(OpenGL REQUIRED)
target_link_libraries(MatGui ${OPENGL_LIBRARIES})
endif()


target_compile_features(MatGui PRIVATE cxx_std_17)


if (BUILD_TESTING)
    enable_testing()

    function(test testname)
        add_executable(${testname}-test tests/${testname}-test.cpp)

        target_include_directories(${testname}-test PRIVATE tests)

        target_link_libraries(${testname}-test
            MatGui
            SDL2_image
            )

        add_test(NAME ${testname}-test
            COMMAND ${testname}-test
            )
    endfunction()

    test(font)
    test(shadertranslate)
    test(signal)
endif()

if (BUILD_DEMOS)
    function(demo name)
        add_executable(${name}-demo demos/${name}-demo.cpp)
        target_link_libraries(${name}-demo
            MatGui
            SDL2_image
            )
    endfunction()

    demo(game)
    demo(minimal)
    demo(multiple-window)
    demo(simple)
    demo(scroll)
endif()
