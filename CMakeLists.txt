cmake_minimum_required(VERSION 3.31...4.4)
project(
    gridmazes
    VERSION 0.0
    DESCRIPTION "Playground for 2D grid mazes"
    LANGUAGES CXX
)

# Dependencies.
find_package(Boost REQUIRED)

# Targets.
add_library(gridmazes)
target_sources(
    gridmazes
    PRIVATE
        src/maze.cpp
        src/generators/kruskal.cpp
        src/properties/connected.cpp
        src/properties/tree.cpp
        src/renderers/text.cpp
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include
            FILES
                include/gridmazes/grid.hpp
                include/gridmazes/maze.hpp
                include/gridmazes/generators/kruskal.hpp
                include/gridmazes/properties/connected.hpp
                include/gridmazes/properties/tree.hpp
                include/gridmazes/renderers/text.hpp
)
target_link_libraries(gridmazes PUBLIC Boost::headers)
target_compile_features(gridmazes PRIVATE cxx_std_23)

if(
    (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
    target_compile_options(gridmazes PRIVATE -Wall -Wpedantic)
elseif(
    (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    OR (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)
    target_compile_options(gridmazes PRIVATE /Wall)
endif()

# Tests.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()
