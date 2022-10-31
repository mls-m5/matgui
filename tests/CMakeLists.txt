
cmake_minimum_required(VERSION 3.22)

enable_testing()

function(test testname)
    add_executable(${testname}-test ${testname}-test.cpp)

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
