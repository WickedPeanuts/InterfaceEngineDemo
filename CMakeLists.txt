cmake_minimum_required(VERSION 3.10)
project(ImGuiTemplate)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)

# --- 1. Dependencies ---

# Tell Raylib not to build examples to keep it fast
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/raylib)

# Define ImGui directory
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)

# --- 2. Source Files ---

# Note: We now use the raylib backend for ImGui instead of the glfw one
# This is usually provided by rlImGui, but if you want to keep it manual:
file(GLOB IMGUI_SOURCES 
    ${IMGUI_DIR}/*.cpp 
    # Since Raylib uses GLFW, the glfw backend can still work 
    # but requires specific setup.
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(${PROJECT_NAME} src/main.cpp ${IMGUI_SOURCES})

# --- 3. Configuration ---

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${IMGUI_DIR} 
    ${IMGUI_DIR}/backends
    external/raylib/src # Access Raylib headers
)

# Link to raylib. It handles OpenGL and GLFW internally.
target_link_libraries(${PROJECT_NAME} PRIVATE 
    raylib
)