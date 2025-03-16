cmake_minimum_required(VERSION 3.22)

function(demo name)
    add_executable(${name}-demo ${name}-demo.cpp)
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
demo(main)

file(
    COPY
    font
    gfx
    DESTINATION
    .
)
