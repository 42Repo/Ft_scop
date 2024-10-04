# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(Scop)

# Définir le standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ajouter les répertoires include/ pour les headers
include_directories(include)

# Ajouter les fichiers sources
add_executable(Scop src/main.cpp src/glad.c)

# Lier les bibliothèques nécessaires pour OpenGL et GLFW
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

target_link_libraries(Scop ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES})
