#include "gridmazes/generators/generator.hpp"
#include "gridmazes/generators/kruskal.hpp"
#include "gridmazes/maze.hpp"
#include "gridmazes/properties/tree.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <random>
#include <utility>

using namespace GridMazes;

TEST_CASE("kruskal's terminates immediately on 1x1 maze", "[kruskal]") {
    const KruskalGenerator generator { 1, 1, std::mt19937 { std::random_device {}() } };
    REQUIRE(generator.has_terminated());
}

TEST_CASE("kruskal's algorithm generates trees", "[kruskal]") {
    const auto& [width, height] {
        GENERATE(std::make_pair(2, 4), std::make_pair(4, 6), std::make_pair(5, 10), std::make_pair(12, 7)),
    };
    const Maze maze { generate<KruskalGenerator>(width, height, std::mt19937 { std::random_device {}() }) };
    REQUIRE(is_tree(maze));
}

TEST_CASE("kruskal's algorithm", "[kruskal]") {
    const auto& [width, height] {
        GENERATE(std::make_pair(2, 4), std::make_pair(4, 6), std::make_pair(5, 10), std::make_pair(12, 7)),
    };
    KruskalGenerator generator { width, height, std::mt19937 { std::random_device {}() } };

    SECTION("generates begins with a filled maze") { REQUIRE(generator.state().all_set()); }
    SECTION("modifies the maze at each step", "[kruskal]") {
        Maze previous { generator.state() };
        while (!generator.has_terminated()) {
            REQUIRE(generator.step() != previous);
        }
    }
}
