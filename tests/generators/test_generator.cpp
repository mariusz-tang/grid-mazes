#include "gridmazes/generators/generator.hpp"
#include "gridmazes/generators/kruskal.hpp"
#include "gridmazes/maze.hpp"
#include "gridmazes/properties/tree.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <random>
#include <utility>

using namespace GridMazes;

TEST_CASE("steps matches generator exactly", "[generator]") {
    const auto& [width, height] {
        GENERATE(std::make_pair(2, 4), std::make_pair(4, 6), std::make_pair(5, 10), std::make_pair(12, 7)),
    };

    std::mt19937 rng { std::random_device {}() };
    std::mt19937 rng_copy { rng };
    KruskalGenerator generator { width, height, rng_copy };

    for (const auto& maze : steps<KruskalGenerator>(width, height, rng)) {
        REQUIRE(maze == generator.state());
        generator.step();
    }
}
