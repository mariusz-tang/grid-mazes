#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"
#include "gridmazes/renderers/text.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

using namespace GridMazes;

TEST_CASE("to_text 1x1 maze", "[to_text]") {
    const std::string_view expectedResult = "XXX\n"
                                            "X X\n"
                                            "XXX";
    const Maze maze { 1, 1 };
    REQUIRE(Render::to_text(maze) == expectedResult);
}

TEST_CASE("to_text 3x1 empty maze", "[to_text]") {
    const std::string_view expectedResult = "XXXXXXX\n"
                                            "X     X\n"
                                            "XXXXXXX";
    const Maze maze { 3, 1 };
    REQUIRE(Render::to_text(maze) == expectedResult);
}

TEST_CASE("to_text 1x3 empty maze", "[to_text]") {
    const std::string_view expectedResult = "XXX\n"
                                            "X X\n"
                                            "X X\n"
                                            "X X\n"
                                            "X X\n"
                                            "X X\n"
                                            "XXX";
    const Maze maze { 1, 3 };
    REQUIRE(Render::to_text(maze) == expectedResult);
}

TEST_CASE("to_text 3x4 empty maze", "[to_text]") {
    constexpr std::string_view expectedResult = "XXXXXXX\n"
                                                "X     X\n"
                                                "X X X X\n"
                                                "X     X\n"
                                                "X X X X\n"
                                                "X     X\n"
                                                "X X X X\n"
                                                "X     X\n"
                                                "XXXXXXX";
    const Maze maze { 3, 4 };
    REQUIRE(Render::to_text(maze) == expectedResult);
}

TEST_CASE("to_text non-empty maze", "[to_text]") {
    constexpr std::string_view expectedResult = "XXXXXXXXXXX\n"
                                                "X     X   X\n"
                                                "XXXXX X XXX\n"
                                                "X   X X   X\n"
                                                "X XXX XXX X\n"
                                                "X   X   X X\n"
                                                "XXX X XXX X\n"
                                                "X         X\n"
                                                "XXXXXXXXXXX";
    const auto width { 5 };
    const auto height { 4 };
    Maze maze { width, height };

    using enum Direction;
    maze.set(Cell { .x = 0, .y = 0 }.wall(down));
    maze.set(Cell { .x = 1, .y = 1 }.wall(up));
    maze.set(Cell { .x = 2, .y = 0 }.wall(right));
    maze.set(Cell { .x = 4, .y = 0 }.wall(down));
    maze.set(Cell { .x = 2, .y = 1 }.wall(left));
    maze.set(Cell { .x = 2, .y = 1 }.wall(right));
    maze.set(Cell { .x = 1, .y = 1 }.wall(down));
    maze.set(Cell { .x = 3, .y = 1 }.wall(down));
    maze.set(Cell { .x = 1, .y = 2 }.wall(right));
    maze.set(Cell { .x = 3, .y = 2 }.wall(right));
    maze.set(Cell { .x = 0, .y = 2 }.wall(down));
    maze.set(Cell { .x = 3, .y = 2 }.wall(down));
    REQUIRE(Render::to_text(maze) == expectedResult);
}
