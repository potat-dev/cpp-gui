cmake_minimum_required(VERSION 3.0)

# initialize the project
project(app)
add_executable(app test.cpp)

add_subdirectory(src)

# link imgui to the executable
target_link_libraries(app gui)