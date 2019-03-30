cmake_minimum_required(VERSION 3.3)
project (OpenGL_Playground)

set(DEPENDENCIES ${PROJECT_SOURCE_DIR}/dependencies)

# OpenGL
find_package(OpenGL REQUIRED)
link_directories(${OPENGL_gl_LIBRARY})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall")

# Add source files and link libraries
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
link_directories(${PROJECT_NAME} ${DEPENDENCIES}/lib)

# Executable will go inside the 'build/bin/' directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} ${SOURCES})

# Include external libraries files
target_include_directories(${PROJECT_NAME} PUBLIC ${DEPENDENCIES}/include)
target_link_libraries(${PROJECT_NAME} glew32 glew32s libglfw3.a libglfw3dll.a ${OPENGL_gl_LIBRARY})

# Copy the DLL files necessary to run the application
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${DEPENDENCIES}/bin"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)