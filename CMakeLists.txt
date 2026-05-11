cmake_minimum_required(VERSION 3.15)
project(OpenGLApp)

set(CMAKE_CXX_STANDARD 17)

# --------------------
# Source files
# --------------------
add_executable(app
    src/main.cpp
    src/Shader.cpp
    src/glad.c
    src/stb_impl.cpp
)

# --------------------
# Include directories
# --------------------
target_include_directories(app PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/include
)

# --------------------
# GLFW library (prebuilt .dylib)
# --------------------
target_link_libraries(app
    ${CMAKE_SOURCE_DIR}/external/lib/libglfw.3.4.dylib

    # macOS frameworks (same as your tasks.json)
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework CoreFoundation"
)