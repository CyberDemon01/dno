cmake_minimum_required(VERSION 3.5)

project(dno)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wall -Wextra -g -Wno-multichar)

#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS})

#find_package(glfw3 REQUIRED)
#include_directories(${GLFW3_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(glfw3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIRS})

add_subdirectory("spdlog")
include_directories("spdlog/include")

#add_subdirectory("glad")
#include_directories("glad/include")

#add_subdirectory("freetype2")
#include_directories("freetype2/include")

add_subdirectory("stb")
include_directories("stb")

add_subdirectory("glm")
include_directories("glm/glm")

#add_subdirectory("assimp")
#include_directories("assimp/include")

find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp
                               window.h
                               window.cpp
                               shader_prog.h
                               shader_prog.cpp
                               event.h
                               event.cpp
                               mesh.h
                               mesh.cpp
                               model.h
                               model.cpp
                               material.h
                               material.cpp
#                               vec4.h
#                               vec3.h
#                               vec2.h
#                               mat4.h
#                               mat3.h
#                               mat2.h
#                               transform.h
#                               math_functions.h
                               utils.h
                               utils.cpp
                               resource/vert.glsl
                               resource/frag.glsl)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} spdlog)
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} stb)
#target_link_libraries(${PROJECT_NAME} glad)
#target_link_libraries(${PROJECT_NAME} freetype)
#target_link_libraries(${PROJECT_NAME} SDL2)
#target_link_libraries(${PROJECT_NAME} assimp)
