#include "gridmazes/grid.hpp"

#include <catch2/catch_test_macros.hpp>
#include <format>
#include <utility>

using namespace GridMazes;
using enum Direction;
using enum Orientation;

TEST_CASE("direction opposite", "[grid]") {
    REQUIRE(opposite(up) == down);
    REQUIRE(opposite(down) == up);
    REQUIRE(opposite(right) == left);
    REQUIRE(opposite(left) == right);
}

TEST_CASE("wall comparison operators", "[grid]") {
    REQUIRE(Wall { 3, 4, horizontal } == Wall { 3, 4, horizontal });
    REQUIRE(Wall { 3, 4, horizontal } != Wall { 3, 4, vertical });
    REQUIRE(Wall { 3, 4, horizontal } != Wall { 2, 4, vertical });
    REQUIRE(Wall { 3, 4, horizontal } != Wall { 3, 3, vertical });
};

TEST_CASE("cell comparison operators", "[grid]") {
    REQUIRE(Cell { 4, 2 } == Cell { 4, 2 });
    REQUIRE(Cell { 4, 2 } != Cell { 2, 2 });
    REQUIRE(Cell { 4, 2 } != Cell { 4, 3 });
};

TEST_CASE("cell translated", "[grid]") {
    REQUIRE(neighbour(Cell { 0, -1 }, up) == Cell { 0, -2 });
    REQUIRE(neighbour(Cell { 1, 1 }, down) == Cell { 1, 2 });
    REQUIRE(neighbour(Cell { 0, -1 }, left) == Cell { -1, -1 });
    REQUIRE(neighbour(Cell { 6, 6 }, right) == Cell { 7, 6 });
}

TEST_CASE("cell wall", "[grid]") {
    REQUIRE(wall(Cell { 0, 0 }, up) == Wall { 0, 0, horizontal });
    REQUIRE(wall(Cell { 2, 1 }, down) == Wall { 2, 2, horizontal });
    REQUIRE(wall(Cell { 0, 0 }, left) == Wall { 0, 0, vertical });
    REQUIRE(wall(Cell { 1, 2 }, right) == Wall { 2, 2, vertical });
}

TEST_CASE("cell same wall from opposite sides", "[grid]") {
    REQUIRE(wall(Cell { 0, 0 }, up) == wall(Cell { 0, -1 }, down));
    REQUIRE(wall(Cell { 3, 2 }, left) == wall(Cell { 2, 2 }, right));
}

TEST_CASE("wall neighbours", "[grid]") {
    REQUIRE(neighbours(Wall { 0, 0, horizontal }) == std::pair { Cell { 0, -1 }, Cell { 0, 0 } });
    REQUIRE(neighbours(Wall { 0, 0, vertical }) == std::pair { Cell { -1, 0 }, Cell { 0, 0 } });
    REQUIRE(neighbours(Wall { 3, 2, horizontal }) == std::pair { Cell { 2, 2 }, Cell { 2, 3 } });
    REQUIRE(neighbours(Wall { 5, 9, vertical }) == std::pair { Cell { 4, 9 }, Cell { 5, 9 } });
}

TEST_CASE("direction formatter", "[grid]") {
    REQUIRE(std::format("{}", up) == "up");
    REQUIRE(std::format("{}", down) == "down");
    REQUIRE(std::format("{}", left) == "left");
    REQUIRE(std::format("{}", right) == "right");
}

TEST_CASE("orientation formatter", "[grid]") {
    REQUIRE(std::format("{}", horizontal) == "horizontal");
    REQUIRE(std::format("{}", vertical) == "vertical");
}

TEST_CASE("wall formatter", "[grid]") { REQUIRE(std::format("{}", Wall { 0, 4, horizontal }) == "[0, 4, horizontal]"); }

TEST_CASE("cell formatter", "[grid]") { REQUIRE(std::format("{}", Cell { 39, 2 }) == "[39, 2]"); }
