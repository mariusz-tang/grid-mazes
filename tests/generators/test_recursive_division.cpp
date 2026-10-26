#include "gridmazes/generators/recursive_division.hpp"
#include "gridmazes/maze.hpp"
#include "gridmazes/properties/tree.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <random>
#include <utility>

using namespace GridMazes;

TEST_CASE("recursive division terminates immediately on 1x1 maze", "[recursive_division]") {
    const RecursiveDivisionGenerator generator { 1, 1, std::mt19937 { std::random_device {}() } };
    REQUIRE(generator.has_terminated());
}

TEST_CASE("recursive division algorithm generates trees", "[recursive_division]") {
    const auto& [width, height] {
        GENERATE(std::make_pair(2, 4), std::make_pair(4, 6), std::make_pair(5, 10), std::make_pair(12, 7)),
    };
    const Maze maze { RecursiveDivisionGenerator { width, height, std::mt19937 { std::random_device {}() } }.run() };
    REQUIRE(is_tree(maze));
}

TEST_CASE("recursive division algorithm", "[recursive_division]") {
    const auto& [width, height] {
        GENERATE(std::make_pair(2, 4), std::make_pair(4, 6), std::make_pair(5, 10), std::make_pair(12, 7)),
    };
    RecursiveDivisionGenerator generator { width, height, std::mt19937 { std::random_device {}() } };

    SECTION("generates begins with an empty maze") { REQUIRE(generator.state().all_unset()); }
    SECTION("modifies the maze at each step", "[recursive_division]") {
        Maze previous { generator.state() };
        while (!generator.has_terminated()) {
            REQUIRE(generator.step() != previous);
        }
    }
}
