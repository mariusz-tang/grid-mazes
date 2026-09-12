#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"
#include "gridmazes/renderers/text.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

using namespace GridMazes;
using enum Direction;

TEST_CASE("to_text 1x1 maze", "[text]") {
    const std::string_view expectedResult { "XXX\n"
                                            "X X\n"
                                            "XXX" };
    const Maze maze { 1, 1 };
    REQUIRE(to_text(maze) == expectedResult);
}

TEST_CASE("to_text 3x1 empty maze", "[text]") {
    const std::string_view expectedResult { "XXXXXXX\n"
                                            "X     X\n"
                                            "XXXXXXX" };
    const Maze maze { 3, 1 };
    REQUIRE(to_text(maze) == expectedResult);
}

TEST_CASE("to_text 1x3 empty maze", "[text]") {
    const std::string_view expectedResult { "XXX\n"
                                            "X X\n"
                                            "X X\n"
                                            "X X\n"
                                            "X X\n"
                                            "X X\n"
                                            "XXX" };
    const Maze maze { 1, 3 };
    REQUIRE(to_text(maze) == expectedResult);
}

TEST_CASE("to_text 3x4 empty maze", "[text]") {
    constexpr std::string_view expectedResult { "XXXXXXX\n"
                                                "X     X\n"
                                                "X X X X\n"
                                                "X     X\n"
                                                "X X X X\n"
                                                "X     X\n"
                                                "X X X X\n"
                                                "X     X\n"
                                                "XXXXXXX" };
    const Maze maze { 3, 4 };
    REQUIRE(to_text(maze) == expectedResult);
}

TEST_CASE("to_text non-empty maze", "[text]") {
    constexpr std::string_view expectedResult { "XXXXXXXXXXX\n"
                                                "X     X   X\n"
                                                "XXXXX X XXX\n"
                                                "X   X X   X\n"
                                                "X XXX XXX X\n"
                                                "X   X   X X\n"
                                                "XXX X XXX X\n"
                                                "X         X\n"
                                                "XXXXXXXXXXX" };
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
    REQUIRE(to_text(maze) == expectedResult);
}

TEST_CASE("parse_text", "[text]") {
    constexpr std::string_view input { "XXXXXXXXX\n"
                                       "X   X   X\n"
                                       "X X X XXX\n"
                                       "X X     X\n"
                                       "X X XXX X\n"
                                       "X   X   X\n"
                                       "XXXXXXXXX" };
    Maze expectedResult { 4, 3 };
    expectedResult.set(Cell { .x = 0, .y = 1 }.wall(right));
    expectedResult.set(Cell { .x = 1, .y = 0 }.wall(right));
    expectedResult.set(Cell { .x = 1, .y = 2 }.wall(right));
    expectedResult.set(Cell { .x = 2, .y = 1 }.wall(down));
    expectedResult.set(Cell { .x = 3, .y = 0 }.wall(down));

    REQUIRE(parse_text(input, 4, 3) == expectedResult);
}

TEST_CASE("parse_text fails if input is not valid", "[text]") {
    SECTION("invalid wall") {
        REQUIRE_FALSE(parse_text("XXXXXXXXX\n"
                                 "X   X   X\n"
                                 "X X X XXX\n"
                                 "X X     X\n"
                                 "X X XXX X\n"
                                 "XX  X   X\n"
                                 "XXXXXXXXX",
                                 4, 3)
                          .has_value());
    }
    SECTION("missing corner") {
        REQUIRE_FALSE(parse_text("XXXXXXXXX\n"
                                 "X   X   X\n"
                                 "X   X XXX\n"
                                 "X X     X\n"
                                 "X X XXX X\n"
                                 "X   X   X\n"
                                 "XXXXXXXXX",
                                 4, 3)
                          .has_value());
    }
    SECTION("missing boundary") {
        REQUIRE_FALSE(parse_text("XX XXXXXX\n"
                                 "X   X   X\n"
                                 "X X X XXX\n"
                                 "X X     X\n"
                                 "X X XXX X\n"
                                 "X   X   X\n"
                                 "XXXXXXXXX",
                                 4, 3)
                          .has_value());
    }
    SECTION("missing newline") {
        REQUIRE_FALSE(parse_text("XXXXXXXXX\n"
                                 "X   X   X\n"
                                 "X X X XXX\n"
                                 "X X     X\n"
                                 "X X XXX X\n"
                                 "X   X   X"
                                 "XXXXXXXXX",
                                 4, 3)
                          .has_value());
    }
    SECTION("extra newline") {
        REQUIRE_FALSE(parse_text("XXXXXXXXX\n"
                                 "X   X   X\n"
                                 "X X X XXX\n"
                                 "X X     X\n"
                                 "X X XXX X\n"
                                 "X   X   X\n"
                                 "XXXXXXXXX\n",
                                 4, 3)
                          .has_value());
    }
    SECTION("wrong maze dimensions") {
        REQUIRE_FALSE(parse_text("XXXXXXXXX\n"
                                 "X   X   X\n"
                                 "X X X XXX\n"
                                 "X X     X\n"
                                 "X X XXX X\n"
                                 "X   X   X\n"
                                 "XXXXXXXXX\n",
                                 3, 4)
                          .has_value());
    }
}

TEST_CASE("parse_text inverts to_text", "[text]") {
    const int width { 5 };
    const int height { 4 };
    Maze maze { width, height };
    maze.set(Cell { .x = 0, .y = 0 }.wall(down));
    maze.set(Cell { .x = 1, .y = 1 }.wall(up));
    maze.set(Cell { .x = 2, .y = 0 }.wall(right));
    maze.set(Cell { .x = 4, .y = 0 }.wall(down));
    maze.set(Cell { .x = 2, .y = 1 }.wall(left));
    maze.set(Cell { .x = 2, .y = 1 }.wall(right));
    maze.set(Cell { .x = 1, .y = 1 }.wall(down));
    maze.set(Cell { .x = 3, .y = 1 }.wall(down));
    maze.set(Cell { .x = 1, .y = 2 }.wall(right));
    REQUIRE(parse_text(to_text(maze), width, height) == maze);
}
