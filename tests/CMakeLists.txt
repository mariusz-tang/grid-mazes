# Install Catch2.
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.15.3
    SYSTEM
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

# Add tests.
add_executable(tests)
target_sources(
    tests
    PRIVATE test_grid.cpp test_maze.cpp renderers/test_text.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain gridmazes)
target_compile_features(tests PRIVATE cxx_std_23)

# Register tests.
include(CTest)
include(Catch)
catch_discover_tests(tests)
