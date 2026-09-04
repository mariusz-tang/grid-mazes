#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iterator>
#include <ranges>
#include <unordered_set>

using namespace GridMazes;
using enum Direction;

TEST_CASE("maze constructor accepts positive width and height", "[maze]") {
    REQUIRE_NOTHROW(Maze { 1, 1 });
    REQUIRE_NOTHROW(Maze { 2, 2 });
    REQUIRE_NOTHROW(Maze { 3, 1 });
    REQUIRE_NOTHROW(Maze { 1, 3 });
    REQUIRE_NOTHROW(Maze { 9, 2 });
}

TEST_CASE("maze constructor throws if either dimension is non-positive", "[maze]") {
    REQUIRE_THROWS(Maze { 1, 0 });
    REQUIRE_THROWS(Maze { 0, 1 });
    REQUIRE_THROWS(Maze { 0, 0 });
    REQUIRE_THROWS(Maze { 10, -1 });
    REQUIRE_THROWS(Maze { -1, 2 });
    REQUIRE_THROWS(Maze { -1, -3 });
}

TEST_CASE("maze width and height", "[maze]") {
    const Maze maze { 4, 2 };
    REQUIRE(maze.width() == 4);
    REQUIRE(maze.height() == 2);
}

TEST_CASE("maze contains cell", "[maze]") {
    const Maze maze { 5, 2 };
    REQUIRE(maze.contains(Cell { 0, 0 }));
    REQUIRE(maze.contains(Cell { 1, 0 }));
    REQUIRE(maze.contains(Cell { 2, 0 }));
    REQUIRE(maze.contains(Cell { 3, 0 }));
    REQUIRE(maze.contains(Cell { 4, 0 }));
    REQUIRE(maze.contains(Cell { 0, 1 }));
    REQUIRE(maze.contains(Cell { 1, 1 }));
    REQUIRE(maze.contains(Cell { 2, 1 }));
    REQUIRE(maze.contains(Cell { 3, 1 }));
    REQUIRE(maze.contains(Cell { 4, 1 }));
    REQUIRE_FALSE(maze.contains(Cell { -5, -5 }));
    REQUIRE_FALSE(maze.contains(Cell { 0, -1 }));
    REQUIRE_FALSE(maze.contains(Cell { -1, 0 }));
    REQUIRE_FALSE(maze.contains(Cell { -1, -1 }));
    REQUIRE_FALSE(maze.contains(Cell { 4, 2 }));
    REQUIRE_FALSE(maze.contains(Cell { 5, 1 }));
    REQUIRE_FALSE(maze.contains(Cell { 5, 2 }));
    REQUIRE_FALSE(maze.contains(Cell { 10, 10 }));
}

TEST_CASE("maze contains wall", "[maze]") {
    const Maze maze { 2, 3 };
    REQUIRE(maze.contains(Cell { 0, 0 }.wall(up)));
    REQUIRE(maze.contains(Cell { 1, 0 }.wall(up)));
    REQUIRE(maze.contains(Cell { 0, 1 }.wall(up)));
    REQUIRE(maze.contains(Cell { 1, 1 }.wall(up)));
    REQUIRE(maze.contains(Cell { 0, 2 }.wall(up)));
    REQUIRE(maze.contains(Cell { 1, 2 }.wall(up)));
    REQUIRE(maze.contains(Cell { 0, 2 }.wall(down)));
    REQUIRE(maze.contains(Cell { 1, 2 }.wall(down)));
    REQUIRE(maze.contains(Cell { 0, 0 }.wall(left)));
    REQUIRE(maze.contains(Cell { 0, 1 }.wall(left)));
    REQUIRE(maze.contains(Cell { 0, 2 }.wall(left)));
    REQUIRE(maze.contains(Cell { 1, 0 }.wall(left)));
    REQUIRE(maze.contains(Cell { 1, 1 }.wall(left)));
    REQUIRE(maze.contains(Cell { 1, 2 }.wall(left)));
    REQUIRE(maze.contains(Cell { 1, 0 }.wall(right)));
    REQUIRE(maze.contains(Cell { 1, 1 }.wall(right)));
    REQUIRE(maze.contains(Cell { 1, 2 }.wall(right)));
    REQUIRE_FALSE(maze.contains(Cell { 0, -1 }.wall(up)));
    REQUIRE_FALSE(maze.contains(Cell { 0, 3 }.wall(down)));
    REQUIRE_FALSE(maze.contains(Cell { -1, 0 }.wall(left)));
    REQUIRE_FALSE(maze.contains(Cell { 3, 2 }.wall(right)));
}

TEST_CASE("maze boundary walls", "[maze]") {
    const Maze maze { 2, 3 };
    REQUIRE(maze.is_boundary(Cell { 0, 0 }.wall(up)));
    REQUIRE(maze.is_boundary(Cell { 1, 0 }.wall(up)));
    REQUIRE(maze.is_boundary(Cell { 0, 2 }.wall(down)));
    REQUIRE(maze.is_boundary(Cell { 1, 2 }.wall(down)));
    REQUIRE(maze.is_boundary(Cell { 0, 0 }.wall(left)));
    REQUIRE(maze.is_boundary(Cell { 0, 1 }.wall(left)));
    REQUIRE(maze.is_boundary(Cell { 0, 2 }.wall(left)));
    REQUIRE(maze.is_boundary(Cell { 1, 0 }.wall(right)));
    REQUIRE(maze.is_boundary(Cell { 1, 1 }.wall(right)));
    REQUIRE(maze.is_boundary(Cell { 1, 2 }.wall(right)));

    // Internal.
    REQUIRE_FALSE(maze.is_boundary(Cell { 0, 1 }.wall(up)));
    REQUIRE_FALSE(maze.is_boundary(Cell { 1, 1 }.wall(up)));
    REQUIRE_FALSE(maze.is_boundary(Cell { 0, 2 }.wall(up)));
    REQUIRE_FALSE(maze.is_boundary(Cell { 1, 2 }.wall(up)));
    REQUIRE_FALSE(maze.is_boundary(Cell { 1, 0 }.wall(left)));
    REQUIRE_FALSE(maze.is_boundary(Cell { 1, 1 }.wall(left)));
    REQUIRE_FALSE(maze.is_boundary(Cell { 1, 2 }.wall(left)));

    // External.
    REQUIRE_FALSE(maze.is_boundary(Cell { 0, -1 }.wall(up)));
    REQUIRE_FALSE(maze.is_boundary(Cell { 0, 3 }.wall(down)));
    REQUIRE_FALSE(maze.is_boundary(Cell { -1, 0 }.wall(left)));
    REQUIRE_FALSE(maze.is_boundary(Cell { 3, 2 }.wall(right)));
}

TEST_CASE("maze internal walls", "[maze]") {
    const Maze maze { 2, 3 };
    REQUIRE(maze.is_internal(Cell { 0, 1 }.wall(up)));
    REQUIRE(maze.is_internal(Cell { 1, 1 }.wall(up)));
    REQUIRE(maze.is_internal(Cell { 0, 2 }.wall(up)));
    REQUIRE(maze.is_internal(Cell { 1, 2 }.wall(up)));
    REQUIRE(maze.is_internal(Cell { 1, 0 }.wall(left)));
    REQUIRE(maze.is_internal(Cell { 1, 1 }.wall(left)));
    REQUIRE(maze.is_internal(Cell { 1, 2 }.wall(left)));

    // Boundary.
    REQUIRE_FALSE(maze.is_internal(Cell { 0, 0 }.wall(up)));
    REQUIRE_FALSE(maze.is_internal(Cell { 1, 0 }.wall(up)));
    REQUIRE_FALSE(maze.is_internal(Cell { 0, 2 }.wall(down)));
    REQUIRE_FALSE(maze.is_internal(Cell { 1, 2 }.wall(down)));
    REQUIRE_FALSE(maze.is_internal(Cell { 0, 0 }.wall(left)));
    REQUIRE_FALSE(maze.is_internal(Cell { 0, 1 }.wall(left)));
    REQUIRE_FALSE(maze.is_internal(Cell { 0, 2 }.wall(left)));
    REQUIRE_FALSE(maze.is_internal(Cell { 1, 0 }.wall(right)));
    REQUIRE_FALSE(maze.is_internal(Cell { 1, 1 }.wall(right)));
    REQUIRE_FALSE(maze.is_internal(Cell { 1, 2 }.wall(right)));

    // External.
    REQUIRE_FALSE(maze.is_internal(Cell { 0, -1 }.wall(up)));
    REQUIRE_FALSE(maze.is_internal(Cell { 0, 3 }.wall(down)));
    REQUIRE_FALSE(maze.is_internal(Cell { -1, 0 }.wall(left)));
    REQUIRE_FALSE(maze.is_internal(Cell { 3, 2 }.wall(right)));
}

TEST_CASE("maze cells generator", "[maze]") {
    const Maze maze { 5, 6 };
    const int numCells { 30 };
    const std::unordered_set uniqueReturnedCells(std::from_range, maze.cells());
    const long numUniqueReturnedCells { static_cast<long>(uniqueReturnedCells.size()) };
    const long numReturnedCells { std::ranges::distance(maze.cells()) };

    SECTION("returns as many cells as there are in the maze") { REQUIRE(numReturnedCells == numCells); }
    SECTION("all cells are unique") { REQUIRE(numUniqueReturnedCells == numReturnedCells); }
    SECTION("the maze contains all cells") {
        for (const auto& cell : maze.cells()) {
            REQUIRE(maze.contains(cell));
        }
    }
}

TEST_CASE("maze walls generator", "[maze]") {
    const Maze maze { 3, 2 };
    const int numWalls { 17 };
    const std::unordered_set uniqueReturnedWalls(std::from_range, maze.walls());
    const long numUniqueReturnedWalls { static_cast<long>(uniqueReturnedWalls.size()) };
    const long numReturnedWalls { std::ranges::distance(maze.walls()) };

    SECTION("returns as many walls as there are in the maze") { REQUIRE(numReturnedWalls == numWalls); }
    SECTION("all walls are unique") { REQUIRE(numUniqueReturnedWalls == numReturnedWalls); }
    SECTION("the maze contains all walls") {
        for (const auto& wall : maze.walls()) {
            REQUIRE(maze.contains(wall));
        }
    }
}

TEST_CASE("maze is_set", "[maze]") {
    const Maze maze { 3, 2 };
    SECTION("boundary walls are set") {
        for (const auto& wall : maze.walls()) {
            if (maze.is_boundary(wall)) {
                REQUIRE(maze.is_set(wall));
            }
        }
    }

    SECTION("external walls are unset") {
        REQUIRE_FALSE(maze.is_set(Cell { -1, 0 }.wall(left)));
        REQUIRE_FALSE(maze.is_set(Cell { -1, -1 }.wall(up)));
        REQUIRE_FALSE(maze.is_set(Cell { 3, 2 }.wall(left)));
    }
}

TEST_CASE("maze setters", "[maze]") {
    Maze maze { 3, 2 };

    SECTION("internal walls can be set, unset, and toggled") {
        for (const auto& wall : maze.walls()) {
            if (!maze.is_internal(wall)) {
                continue;
            }
            REQUIRE_FALSE(maze.is_set(wall));
            maze.set(wall);
            REQUIRE(maze.is_set(wall));
            maze.unset(wall);
            REQUIRE_FALSE(maze.is_set(wall));
            maze.toggle(wall);
            REQUIRE(maze.is_set(wall));
            maze.toggle(wall);
            REQUIRE_FALSE(maze.is_set(wall));
        }
    }
    SECTION("walls do not affect each other") {
        const Wall myWall { Cell { .x = 0, .y = 0 }.wall(right) };
        maze.set(myWall);
        REQUIRE(maze.is_set(myWall));

        for (const auto& wall : maze.walls()) {
            if (maze.is_internal(wall) && wall != myWall) {
                REQUIRE_FALSE(maze.is_set(wall));
            }
        }
    }
}

TEST_CASE("maze set_all", "[maze]") {
    Maze maze { 15, 4 }; // NOLINT (magic numbers)

    for (const auto& wall : maze.walls()) {
        if (maze.is_internal(wall)) {
            REQUIRE_FALSE(maze.is_set(wall));
        }
    }
    maze.set_all();
    for (const auto& wall : maze.walls()) {
        if (maze.is_internal(wall)) {
            REQUIRE(maze.is_set(wall));
        }
    }
}

TEST_CASE("maze unset_all", "[maze]") {
    Maze maze { 15, 4 }; // NOLINT (magic numbers)

    maze.set_all();
    for (const auto& wall : maze.walls()) {
        if (maze.is_internal(wall)) {
            REQUIRE(maze.is_set(wall));
        }
    }

    maze.unset_all();
    for (const auto& wall : maze.walls()) {
        if (maze.is_internal(wall)) {
            REQUIRE_FALSE(maze.is_set(wall));
        }
    }
}

TEST_CASE("maze constructor produces an empty maze", "[maze]") {
    const Maze maze { 10, 3 };
    REQUIRE(maze.all_unset());
    REQUIRE_FALSE(maze.all_set());
}

TEST_CASE("setting any walls makes the maze non-empty", "[maze]") {
    Maze maze { 10, 3 }; // NOLINT (magic numbers)

    REQUIRE(maze.all_unset());
    maze.set(Cell { .x = 0, .y = 0 }.wall(right));
    REQUIRE_FALSE(maze.all_unset());
}

TEST_CASE("unsetting all walls makes the maze empty", "[maze]") {
    Maze maze { 2, 3 }; // NOLINT (magic numbers)

    maze.set_all();
    REQUIRE_FALSE(maze.all_unset());
    maze.unset_all();
    REQUIRE(maze.all_unset());
}

TEST_CASE("setting all walls makes the maze full", "[maze]") {
    Maze maze { 10, 3 }; // NOLINT (magic numbers)
    REQUIRE_FALSE(maze.all_set());
    maze.set_all();
    REQUIRE(maze.all_set());
}
