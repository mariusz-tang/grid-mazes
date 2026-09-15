#include "gridmazes/generators/kruskal.hpp"
#include "gridmazes/maze.hpp"
#include "gridmazes/properties/tree.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <random>
#include <utility>

using namespace GridMazes;

TEST_CASE("kruskal's algorithm generates trees", "[kruskal]") {
    const auto& [width, height] { GENERATE(std::make_pair(2, 4), std::make_pair(4, 6), std::make_pair(5, 10),
                                           std::make_pair(12, 7)) };
    const Maze maze { kruskal(width, height, std::mt19937 { std::random_device {}() }) };
    REQUIRE(is_tree(maze));
}
