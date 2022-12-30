cmake_minimum_required(VERSION 3.10)
project(ccgame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(CCGAME_WITH_EXAMPLE "Compile ccgame example" OFF)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(ccgame
    src/sdl.cc
    src/window.cc
    src/context.cc
    src/timer.cc
    src/view.cc
    src/view/textureview.cc
    src/view/imageview.cc
    src/view/textview.cc
    src/view/paintview.cc
    src/view/rectview.cc
)

target_include_directories(ccgame INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ccgame
    PUBLIC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

#install the example
if(CCGAME_WITH_EXAMPLE)
    add_subdirectory(example/snake)
    add_subdirectory(example/circle)
endif()
