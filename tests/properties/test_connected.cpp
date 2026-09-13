#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"
#include "gridmazes/properties/connected.hpp"
#include "gridmazes/renderers/text.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <utility>

using namespace GridMazes;

TEST_CASE("empty mazes are connected", "[connected]") {
    const Maze maze { GENERATE(Maze { 1, 1 }, Maze { 3, 5 }, Maze { 10, 10 }) };
    REQUIRE(is_connected(maze));
}

TEST_CASE("filled mazes are not connected", "[connected]") {
    Maze maze { GENERATE(Maze { 3, 5 }, Maze { 10, 10 }) };
    maze.set_all();
    REQUIRE_FALSE(is_connected(maze));
}

TEST_CASE("connected mazes", "[connected]") {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const Maze maze { GENERATE(parse_text("XXXXXXX\n"
                                          "X     X\n"
                                          "X XXX X\n"
                                          "X   X X\n"
                                          "XXXXXXX",
                                          3, 2),
                               parse_text("XXXXXXXXX\n"
                                          "X       X\n"
                                          "X X XXX X\n"
                                          "X       X\n"
                                          "X XXX X X\n"
                                          "X       X\n"
                                          "XXX X X X\n"
                                          "X       X\n"
                                          "XXXXXXXXX",
                                          4, 4),
                               parse_text("XXXXXXXXXXX\n"
                                          "X         X\n"
                                          "X X XXX X X\n"
                                          "X X X   X X\n"
                                          "X XXXXX XXX\n"
                                          "X X       X\n"
                                          "XXX XXX XXX\n"
                                          "X     X   X\n"
                                          "XXXXXXXXXXX",
                                          5, 4))
                          .value() };
    REQUIRE(is_connected(maze));
}

TEST_CASE("disconnected mazes", "[connected]") {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const Maze maze { GENERATE(parse_text("XXXXXXX\n"
                                          "X X   X\n"
                                          "X XXX X\n"
                                          "X   X X\n"
                                          "XXXXXXX",
                                          3, 2),
                               parse_text("XXXXXXXXX\n"
                                          "X   X   X\n"
                                          "X X XXX X\n"
                                          "X   X   X\n"
                                          "X XXX X X\n"
                                          "X   X   X\n"
                                          "XXX X X X\n"
                                          "X   X   X\n"
                                          "XXXXXXXXX",
                                          4, 4),
                               parse_text("XXXXXXXXXXX\n"
                                          "X   X     X\n"
                                          "X X XXX X X\n"
                                          "X X X   X X\n"
                                          "X XXXXX XXX\n"
                                          "X X       X\n"
                                          "XXX XXX XXX\n"
                                          "X     X   X\n"
                                          "XXXXXXXXXXX",
                                          5, 4))
                          .value() };
    REQUIRE_FALSE(is_connected(maze));
}

TEST_CASE("connected cells", "[connected]") {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const Maze maze { parse_text("XXXXXXXXXXX\n"
                                 "X   X     X\n"
                                 "X X XXX X X\n"
                                 "X X X   X X\n"
                                 "X XXXXX XXX\n"
                                 "X X       X\n"
                                 "XXX XXX XXX\n"
                                 "X     X   X\n"
                                 "XXXXXXXXXXX",
                                 5, 4)
                          .value() };
    const auto& [start, finish] { GENERATE(
        std::make_pair(Cell { 0, 0 }, Cell { 0, 0 }), std::make_pair(Cell { 0, 0 }, Cell { 0, 2 }),
        std::make_pair(Cell { 0, 0 }, Cell { 1, 1 }), std::make_pair(Cell { 3, 0 }, Cell { 4, 3 }),
        std::make_pair(Cell { 0, 3 }, Cell { 2, 2 }), std::make_pair(Cell { 2, 2 }, Cell { 2, 2 })) };
    REQUIRE(is_connected(maze, start, finish));
}

TEST_CASE("disconnected cells", "[connected]") {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const Maze maze { parse_text("XXXXXXXXXXX\n"
                                 "X   X     X\n"
                                 "X X XXX X X\n"
                                 "X X X   X X\n"
                                 "X XXXXX XXX\n"
                                 "X X       X\n"
                                 "XXX XXX XXX\n"
                                 "X     X   X\n"
                                 "XXXXXXXXXXX",
                                 5, 4)
                          .value() };
    const auto& [start, finish] { GENERATE(
        std::make_pair(Cell { 0, 0 }, Cell { 4, 3 }), std::make_pair(Cell { 2, 2 }, Cell { 0, 2 }),
        std::make_pair(Cell { 3, 3 }, Cell { 1, 1 }), std::make_pair(Cell { 3, 0 }, Cell { 0, 1 }),
        std::make_pair(Cell { 0, 3 }, Cell { 1, 0 }), std::make_pair(Cell { 0, 2 }, Cell { 2, 2 })) };
    REQUIRE_FALSE(is_connected(maze, start, finish));
}
