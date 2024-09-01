cmake_minimum_required(VERSION 3.5.0)
project(2d_engine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    main.cpp
    glad.c
)

add_executable(2d_engine ${SOURCES})

target_link_libraries(2d_engine
    PRIVATE
    glfw
    glm
)
