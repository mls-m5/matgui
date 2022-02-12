
cmake_minimum_required(VERSION 3.22)

project(MatGui)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

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
    ${OPENGL_LIBRARIES}
    )

target_compile_features(MatGui PRIVATE cxx_std_17)

