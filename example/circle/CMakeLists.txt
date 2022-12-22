cmake_minimum_required(VERSION 3.10)

project(Circle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
    main.cc
)
target_link_libraries(${PROJECT_NAME} PRIVATE ccgame)