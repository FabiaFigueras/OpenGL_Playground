cmake_minimum_required(VERSION 3.0)
project (OpenGL_Test)

# This command lets us include multiple files using wildcards
file(GLOB SOURCES "src/*.cpp")

# Executable will go inside the 'build/bin/' directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} ${SOURCES})