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
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
)

# --------------------
# Include directories
# --------------------
target_include_directories(app PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/include
    ${CMAKE_SOURCE_DIR}/imgui
    /Users/jathavanmahendrarajah/assimp/include
)

# --------------------
# GLFW library (prebuilt .dylib)
# --------------------
target_link_libraries(app
    ${CMAKE_SOURCE_DIR}/external/lib/libglfw.3.4.dylib
    /Users/jathavanmahendrarajah/assimp/bin/assimp.framework/assimp

    # macOS frameworks (same as your tasks.json)
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework CoreFoundation"
)