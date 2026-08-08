cmake_minimum_required(VERSION 3.31...4.4)
project(
    GridMazes
    VERSION 0.0
    DESCRIPTION "Playground for 2D grid mazes"
    LANGUAGES CXX
)

# Dependencies.
find_package(Boost REQUIRED)

# Targets.
add_library(GridMazes)
target_sources(
    GridMazes
    PRIVATE src/maze.cpp src/position.cpp
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include
            FILES include/gridmazes/maze.hpp include/gridmazes/position.hpp
)
target_link_libraries(GridMazes PUBLIC Boost::headers)
target_compile_features(GridMazes PRIVATE cxx_std_23)

# Compiler warnings.
option(GRIDMAZES_WARNINGS_ARE_ERRORS "treat warnings as errors" ON)

if(
    (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
    target_compile_options(GridMazes PRIVATE -Wall -Wpedantic)
    if(GRIDMAZES_WARNINGS_ARE_ERRORS)
        target_compile_options(GridMazes PRIVATE -Werror)
    endif()
elseif(
    (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    OR (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)
    target_compile_options(GridMazes PRIVATE /Wall)
    if(GRIDMAZES_WARNINGS_ARE_ERRORS)
        target_compile_options(GridMazes PRIVATE /WX)
    endif()
endif()

# Tests.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()
