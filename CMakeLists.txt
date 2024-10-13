cmake_minimum_required(VERSION 3.10)

project(Scop)

message ("${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0072 NEW)

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/images DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/Models DESTINATION ${CMAKE_BINARY_DIR})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default: Release)" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Werror -Wextra -g -gdwarf-4 -Wunused -Wconversion -Wfloat-equal -Wnon-virtual-dtor \
        -Wpointer-arith -Wstrict-aliasing=2 -Wunreachable-code -Wnull-dereference \
        -Wnonnull -Wpedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy \
        -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations \
        -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual \
        -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
        -Wstrict-overflow=5 -Wswitch-default -Wundef")
    
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Weverything -Wno-reserved-macro-identifier -Wno-reserved-identifier -Wno-c++98-compat -Wno-cast-function-type-strict -Wno-padded")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wlogical-op -Wstrict-null-sentinel")
    endif()
endif()

include_directories(include)

set(SRCS
    main.cpp
    src/glad.c
    src/Shader.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/Texture.cpp
    src/InputHandler.cpp
    src/Scene.cpp
    src/ObjLoader.cpp
    include/add_images_lib.cpp
)

add_executable(Scop ${SRCS})

set_source_files_properties(include/add_images_lib.cpp PROPERTIES COMPILE_FLAGS "-Wno-error -Wno-all -Wno-cast-qual -Wno-old-style-cast -Wno-sign-conversion -Wno-conversion -Wno-switch-default -Wno-strict-overflow -Wno-everything")

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

target_link_libraries(Scop ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES})
