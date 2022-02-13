
cmake_minimum_required(VERSION 3.22)


project(MatGui)

find_package(SDL2 REQUIRED)

add_library(MatGui matgui.cpp)

target_include_directories(
    MatGui
    PUBLIC
       include
       ${SDL2_INCLUDE_DIRS}
       ${OPENGL_INCLUDE_DIR}
       SDL2::Image
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

