#include "gridmazes/maze.hpp"
#include "gridmazes/properties/tree.hpp"
#include "gridmazes/renderers/text.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

using namespace GridMazes;

TEST_CASE("1 by 1 maze is a tree", "[tree]") { REQUIRE(is_tree(Maze { 1, 1 })); }

TEST_CASE("empty mazes are not trees", "[tree]") {
    const Maze maze { GENERATE(Maze { 2, 2 }, Maze { 4, 6 }, Maze { 9, 8 }) };
    REQUIRE_FALSE(is_tree(maze));
}

TEST_CASE("filled mazes are not trees", "[tree]") {
    Maze maze { GENERATE(Maze { 2, 2 }, Maze { 4, 6 }, Maze { 9, 8 }) };
    maze.set_all();
    REQUIRE_FALSE(is_tree(maze));
}

TEST_CASE("non-trees", "[connected]") {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const Maze maze { GENERATE(parse_text("XXXXXXX\n"
                                          "X     X\n"
                                          "X X X X\n"
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
                                          "X X XXX XXX\n"
                                          "X     X   X\n"
                                          "XXXXXXXXXXX",
                                          5, 4))
                          .value() };
    REQUIRE_FALSE(is_tree(maze));
}

TEST_CASE("trees", "[connected]") {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const Maze maze { GENERATE(parse_text("XXXXXXX\n"
                                          "X     X\n"
                                          "X XXX X\n"
                                          "X   X X\n"
                                          "XXXXXXX",
                                          3, 2),
                               parse_text("XXXXXXXXXXX\n"
                                          "X         X\n"
                                          "X X XXX X X\n"
                                          "X X X   X X\n"
                                          "X XXXXX XXX\n"
                                          "X X       X\n"
                                          "X XXXXX XXX\n"
                                          "X     X   X\n"
                                          "XXXXXXXXXXX",
                                          5, 4))
                          .value() };
    REQUIRE(is_tree(maze));
}
