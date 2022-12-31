cmake_minimum_required(VERSION 3.0)
include(FetchContent)

# initialize the project
project(gui)
add_library(gui STATIC gui.cpp gui.hpp render.cpp)

# find and make available OpenGL
find_package(OpenGL)

# fetch and make available glad
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG df8e9e16110b305479a875399cee13daa0ccadd9
)
FetchContent_MakeAvailable(glad)

# disable glfw examples, tests, docs and install
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# fetch and make available glfw
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.2
)
FetchContent_MakeAvailable(glfw)

# fetch and make available imgui (docking branch)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_GetProperties(imgui)

# populate imgui and link it to the executable
if (NOT imgui_POPULATED)

    FetchContent_Populate(imgui)
    
    file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp)
    file(GLOB IMGUI_HEADERS ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

    # create imgui library
    add_library(
        imgui STATIC
        ${IMGUI_SOURCES}
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    
    # include imgui, glfw, glad and opengl headers
    target_include_directories(
        imgui PUBLIC
        ${IMGUI_HEADERS}
        ${GLFW_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
        ${OPENGL_INCLUDE_DIR}
    )
    
    # link opengl, glfw and glad to imgui
    target_link_libraries(imgui ${OPENGL_LIBRARIES} glfw glad)

endif()

# link imgui to the executable
target_link_libraries(gui imgui)