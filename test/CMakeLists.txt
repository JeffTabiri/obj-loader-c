cmake_minimum_required(VERSION 4.0)

project(obj_viewer C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Find glfw3 and OpenGL
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

#add all resources/shaders
file(COPY shader.frag shader.vert cube.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include_directories(include)

add_executable(obj_viewer main.cpp camera.cpp glad.c Shader.cpp)

target_link_libraries(obj_viewer PUBLIC glfw OpenGL::GL)
