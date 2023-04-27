cmake_minimum_required(VERSION 3.1)
project(GeometricScene)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_LIB LengthSumVisitor.cpp Shape.cpp GeometricScene.cpp SceneElement.cpp)
set(HEADER_LIB LengthSumVisitor.hpp Shape.hpp GeometricScene.hpp SceneElement.hpp)
set(SOURCE_EXE main.cpp)

add_library(lib SHARED ${SOURCE_LIB} ${HEADER_LIB})
add_executable(GeomScene ${SOURCE_EXE})

target_link_libraries(GeomScene lib)
