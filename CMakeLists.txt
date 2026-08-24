cmake_minimum_required(VERSION 3.24)

project(idk_physics LANGUAGES CXX)

option(BUILD_UI "Build the ImGui/ImPlot3D UI (off = headless sim)" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
find_package(OpenGL REQUIRED)

# --- sim core (no UI dependencies) ---

add_library(idk_sim STATIC
    src/sim/Sim.cpp
)
target_include_directories(idk_sim PUBLIC src)
target_compile_options(idk_sim PRIVATE -Wall -Wextra)

# --- UI (ImGui + ImPlot3D + GLFW), only when requested ---

if(BUILD_UI)
    FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.9
        GIT_SHALLOW TRUE
    )
    FetchContent_Declare(implot3d
        GIT_REPOSITORY https://github.com/brenocq/implot3d.git
        GIT_TAG v0.4
        GIT_SHALLOW TRUE
    )
    FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
    )
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(imgui implot3d glfw)

    add_library(idk_ui STATIC
        src/ui/Ui.cpp
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${implot3d_SOURCE_DIR}/implot3d.cpp
        ${implot3d_SOURCE_DIR}/implot3d_items.cpp
        ${implot3d_SOURCE_DIR}/implot3d_meshes.cpp
    )
    target_include_directories(idk_ui PUBLIC
        src
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${implot3d_SOURCE_DIR}
    )
    target_include_directories(idk_ui PRIVATE config)
    target_compile_definitions(idk_ui PRIVATE IMGUI_USER_CONFIG="idk_imconfig.h")
    target_link_libraries(idk_ui PUBLIC
        idk_sim
        glfw
        OpenGL::GL
    )
    target_compile_options(idk_ui PRIVATE -Wall -Wextra)
endif()

# --- app ---

add_executable(idk_physics
    src/main.cpp
    src/app/App.cpp
)
target_include_directories(idk_physics PRIVATE src)
target_link_libraries(idk_physics PRIVATE idk_sim)
if(BUILD_UI)
    target_link_libraries(idk_physics PRIVATE idk_ui)
    target_compile_definitions(idk_physics PRIVATE BUILD_UI=1)
endif()
target_compile_options(idk_physics PRIVATE -Wall -Wextra)