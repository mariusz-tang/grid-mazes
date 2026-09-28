# Dependencies.
include(FetchContent)
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.7.2
    SYSTEM
)
FetchContent_MakeAvailable(cli11)

# Targets.
add_executable(gridmazes-cli)
target_sources(gridmazes-cli PRIVATE main.cpp random.h)
target_link_libraries(gridmazes-cli PRIVATE gridmazes CLI11::CLI11)
target_compile_features(gridmazes PRIVATE cxx_std_23)

if(
    (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
    target_compile_options(gridmazes-cli PRIVATE -Wall -Wpedantic)
elseif(
    (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    OR (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)
    target_compile_options(gridmazes-cli PRIVATE /Wall)
endif()
