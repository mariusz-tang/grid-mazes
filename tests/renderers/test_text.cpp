#include "gridmazes/maze.hpp"
#include "gridmazes/position.hpp"
#include "gridmazes/renderers/text.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("to_text 1x1 maze", "[to_text]") {
    const std::string_view expectedResult = "XXX\n"
                                            "X X\n"
                                            "XXX";
    const GridMazes::Maze maze { 1, 1 };
    REQUIRE(GridMazes::Render::to_text(maze) == expectedResult);
}

TEST_CASE("to_text 3x1 empty maze", "[to_text]") {
    const std::string_view expectedResult = "XXXXXXX\n"
                                            "X     X\n"
                                            "XXXXXXX";
    const GridMazes::Maze maze { 3, 1 };
    REQUIRE(GridMazes::Render::to_text(maze) == expectedResult);
}

TEST_CASE("to_text 1x3 empty maze", "[to_text]") {
    const std::string_view expectedResult = "XXX\n"
                                            "X X\n"
                                            "X X\n"
                                            "X X\n"
                                            "X X\n"
                                            "X X\n"
                                            "XXX";
    const GridMazes::Maze maze { 1, 3 };
    REQUIRE(GridMazes::Render::to_text(maze) == expectedResult);
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
    const GridMazes::Maze maze { 3, 4 };
    REQUIRE(GridMazes::Render::to_text(maze) == expectedResult);
}

TEST_CASE("non-empty maze", "[to_text]") {
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
    GridMazes::Maze maze { width, height };

    using enum GridMazes::Direction;
    maze.place_wall({ .column = 0, .row = 0 }, down);
    maze.place_wall({ .column = 1, .row = 1 }, up);
    maze.place_wall({ .column = 2, .row = 0 }, right);
    maze.place_wall({ .column = 4, .row = 0 }, down);
    maze.place_wall({ .column = 2, .row = 1 }, left);
    maze.place_wall({ .column = 2, .row = 1 }, right);
    maze.place_wall({ .column = 1, .row = 1 }, down);
    maze.place_wall({ .column = 3, .row = 1 }, down);
    maze.place_wall({ .column = 1, .row = 2 }, right);
    maze.place_wall({ .column = 3, .row = 2 }, right);
    maze.place_wall({ .column = 0, .row = 2 }, down);
    maze.place_wall({ .column = 3, .row = 2 }, down);
    REQUIRE(GridMazes::Render::to_text(maze) == expectedResult);
}
