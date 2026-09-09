#include "gridmazes/grid.hpp"

#include <catch2/catch_test_macros.hpp>
#include <utility>

using namespace GridMazes;
using enum Direction;
using enum Orientation;

TEST_CASE("direction opposite", "[grid]") {
    REQUIRE(get_opposite(up) == down);
    REQUIRE(get_opposite(down) == up);
    REQUIRE(get_opposite(right) == left);
    REQUIRE(get_opposite(left) == right);
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
    REQUIRE(Cell { 0, -1 }.translated(up) == Cell { 0, -2 });
    REQUIRE(Cell { 1, 1 }.translated(down, 2) == Cell { 1, 3 });
    REQUIRE(Cell { 0, -1 }.translated(left, 4) == Cell { -4, -1 });
    REQUIRE(Cell { 6, 6 }.translated(right, 3) == Cell { 9, 6 });

    REQUIRE(Cell { 0, -1 }.translated(up, 0) == Cell { 0, -1 });
    REQUIRE(Cell { 2, 3 }.translated(up, -3) == Cell { 2, 6 });
}

TEST_CASE("cell wall", "[grid]") {
    REQUIRE(Cell { 0, 0 }.wall(up) == Wall { 0, 0, horizontal });
    REQUIRE(Cell { 2, 1 }.wall(down) == Wall { 2, 2, horizontal });
    REQUIRE(Cell { 0, 0 }.wall(left) == Wall { 0, 0, vertical });
    REQUIRE(Cell { 1, 2 }.wall(right) == Wall { 2, 2, vertical });
}

TEST_CASE("cell same wall from opposite sides", "[grid]") {
    REQUIRE(Cell { 0, 0 }.wall(up) == Cell { 0, -1 }.wall(down));
    REQUIRE(Cell { 3, 2 }.wall(left) == Cell { 2, 2 }.wall(right));
}

TEST_CASE("wall cells", "[grid]") {
    REQUIRE(Wall { 0, 0, horizontal }.cells() == std::pair { Cell { 0, -1 }, Cell { 0, 0 } });
    REQUIRE(Wall { 0, 0, vertical }.cells() == std::pair { Cell { -1, 0 }, Cell { 0, 0 } });
    REQUIRE(Wall { 3, 2, horizontal }.cells() == std::pair { Cell { 2, 2 }, Cell { 2, 3 } });
    REQUIRE(Wall { 5, 9, vertical }.cells() == std::pair { Cell { 4, 9 }, Cell { 5, 9 } });
}
