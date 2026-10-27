#include "gridmazes/generators/prim.hpp"
#include "gridmazes/maze.hpp"
#include "gridmazes/properties/tree.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <random>
#include <utility>

using namespace GridMazes;

TEST_CASE("Prim's terminates immediately on 1x1 maze", "[prim]") {
    const PrimGenerator generator { 1, 1, std::mt19937 { std::random_device {}() } };
    REQUIRE(generator.has_terminated());
}

TEST_CASE("Prim's algorithm generates trees", "[prim]") {
    const auto& [width, height] {
        GENERATE(std::make_pair(2, 4), std::make_pair(4, 6), std::make_pair(5, 10), std::make_pair(12, 7)),
    };
    const Maze maze { PrimGenerator { width, height, std::mt19937 { std::random_device {}() } }.run() };
    REQUIRE(is_tree(maze));
}

TEST_CASE("Prim's algorithm", "[prim]") {
    const auto& [width, height] {
        GENERATE(std::make_pair(2, 4), std::make_pair(4, 6), std::make_pair(5, 10), std::make_pair(12, 7)),
    };
    PrimGenerator generator { width, height, std::mt19937 { std::random_device {}() } };

    SECTION("begins with a filled maze") { REQUIRE(generator.state().all_set()); }
    SECTION("modifies the maze at each step", "[prim]") {
        Maze previous { generator.state() };
        while (!generator.has_terminated()) {
            REQUIRE(generator.step() != previous);
        }
    }
}
