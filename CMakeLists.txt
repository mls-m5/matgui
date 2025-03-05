
cmake_minimum_required(VERSION 3.22)

project(MatGui)

add_library(MatGui SHARED matgui.cpp)

add_subdirectory(lib/sdlpp)
add_subdirectory(lib/glpp)

target_include_directories(
    MatGui
    PUBLIC
       include
       ${OPENGL_INCLUDE_DIR}
       SDL2_image
    PRIVATE
       src
    )

target_link_libraries(
    MatGui
    sdlpp
    glpp
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


if (NOT NO_BUILD_TESTING)
    add_subdirectory(tests)
endif()

if (NOT NO_BUILD_DEMOS)
    add_subdirectory(demos)
endif()
