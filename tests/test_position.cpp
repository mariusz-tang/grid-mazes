#include "gridmazes/position.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("position get_neighbour") {
    REQUIRE(GridMazes::Position { 0, 0 }.get_neighbour(GridMazes::Direction::up) == GridMazes::Position { 0, -1 });
    REQUIRE(GridMazes::Position { 10, 4 }.get_neighbour(GridMazes::Direction::down) == GridMazes::Position { 10, 5 });
    REQUIRE(GridMazes::Position { 4, 9 }.get_neighbour(GridMazes::Direction::left) == GridMazes::Position { 3, 9 });
    REQUIRE(GridMazes::Position { -20, 20 }.get_neighbour(GridMazes::Direction::right) ==
            GridMazes::Position { -19, 20 });
}
