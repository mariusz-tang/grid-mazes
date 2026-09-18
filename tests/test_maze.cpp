#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"
#include "gridmazes/renderers/text.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <format>
#include <iterator>
#include <ranges>
#include <unordered_set>
#include <utility>

using namespace GridMazes;
using enum Direction;
using enum Orientation;

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

TEST_CASE("maze basic properties", "[maze]") {
    const Maze maze { 4, 2 };
    REQUIRE(maze.width() == 4);
    REQUIRE(maze.height() == 2);
    REQUIRE(maze.size() == 8);
}

TEST_CASE("maze contains cell", "[maze]") {
    const Maze maze { 5, 2 };
    REQUIRE(contains(maze, Cell { 0, 0 }));
    REQUIRE(contains(maze, Cell { 1, 0 }));
    REQUIRE(contains(maze, Cell { 2, 0 }));
    REQUIRE(contains(maze, Cell { 3, 0 }));
    REQUIRE(contains(maze, Cell { 4, 0 }));
    REQUIRE(contains(maze, Cell { 0, 1 }));
    REQUIRE(contains(maze, Cell { 1, 1 }));
    REQUIRE(contains(maze, Cell { 2, 1 }));
    REQUIRE(contains(maze, Cell { 3, 1 }));
    REQUIRE(contains(maze, Cell { 4, 1 }));
    REQUIRE_FALSE(contains(maze, Cell { -5, -5 }));
    REQUIRE_FALSE(contains(maze, Cell { 0, -1 }));
    REQUIRE_FALSE(contains(maze, Cell { -1, 0 }));
    REQUIRE_FALSE(contains(maze, Cell { -1, -1 }));
    REQUIRE_FALSE(contains(maze, Cell { 4, 2 }));
    REQUIRE_FALSE(contains(maze, Cell { 5, 1 }));
    REQUIRE_FALSE(contains(maze, Cell { 5, 2 }));
    REQUIRE_FALSE(contains(maze, Cell { 10, 10 }));
}

TEST_CASE("maze contains wall", "[maze]") {
    const Maze maze { 2, 3 };
    REQUIRE(contains(maze, wall(Cell { 0, 0 }, up)));
    REQUIRE(contains(maze, wall(Cell { 1, 0 }, up)));
    REQUIRE(contains(maze, wall(Cell { 0, 1 }, up)));
    REQUIRE(contains(maze, wall(Cell { 1, 1 }, up)));
    REQUIRE(contains(maze, wall(Cell { 0, 2 }, up)));
    REQUIRE(contains(maze, wall(Cell { 1, 2 }, up)));
    REQUIRE(contains(maze, wall(Cell { 0, 2 }, down)));
    REQUIRE(contains(maze, wall(Cell { 1, 2 }, down)));
    REQUIRE(contains(maze, wall(Cell { 0, 0 }, left)));
    REQUIRE(contains(maze, wall(Cell { 0, 1 }, left)));
    REQUIRE(contains(maze, wall(Cell { 0, 2 }, left)));
    REQUIRE(contains(maze, wall(Cell { 1, 0 }, left)));
    REQUIRE(contains(maze, wall(Cell { 1, 1 }, left)));
    REQUIRE(contains(maze, wall(Cell { 1, 2 }, left)));
    REQUIRE(contains(maze, wall(Cell { 1, 0 }, right)));
    REQUIRE(contains(maze, wall(Cell { 1, 1 }, right)));
    REQUIRE(contains(maze, wall(Cell { 1, 2 }, right)));
    REQUIRE_FALSE(contains(maze, wall(Cell { 0, -1 }, up)));
    REQUIRE_FALSE(contains(maze, wall(Cell { 0, 3 }, down)));
    REQUIRE_FALSE(contains(maze, wall(Cell { -1, 0 }, left)));
    REQUIRE_FALSE(contains(maze, wall(Cell { 3, 2 }, right)));
}

TEST_CASE("maze boundary walls", "[maze]") {
    const Maze maze { 2, 3 };
    REQUIRE(is_boundary(wall(Cell { 0, 0 }, up), maze));
    REQUIRE(is_boundary(wall(Cell { 1, 0 }, up), maze));
    REQUIRE(is_boundary(wall(Cell { 0, 2 }, down), maze));
    REQUIRE(is_boundary(wall(Cell { 1, 2 }, down), maze));
    REQUIRE(is_boundary(wall(Cell { 0, 0 }, left), maze));
    REQUIRE(is_boundary(wall(Cell { 0, 1 }, left), maze));
    REQUIRE(is_boundary(wall(Cell { 0, 2 }, left), maze));
    REQUIRE(is_boundary(wall(Cell { 1, 0 }, right), maze));
    REQUIRE(is_boundary(wall(Cell { 1, 1 }, right), maze));
    REQUIRE(is_boundary(wall(Cell { 1, 2 }, right), maze));

    // Internal.
    REQUIRE_FALSE(is_boundary(wall(Cell { 0, 1 }, up), maze));
    REQUIRE_FALSE(is_boundary(wall(Cell { 1, 1 }, up), maze));
    REQUIRE_FALSE(is_boundary(wall(Cell { 0, 2 }, up), maze));
    REQUIRE_FALSE(is_boundary(wall(Cell { 1, 2 }, up), maze));
    REQUIRE_FALSE(is_boundary(wall(Cell { 1, 0 }, left), maze));
    REQUIRE_FALSE(is_boundary(wall(Cell { 1, 1 }, left), maze));
    REQUIRE_FALSE(is_boundary(wall(Cell { 1, 2 }, left), maze));

    // External.
    REQUIRE_FALSE(is_boundary(wall(Cell { 0, -1 }, up), maze));
    REQUIRE_FALSE(is_boundary(wall(Cell { 0, 3 }, down), maze));
    REQUIRE_FALSE(is_boundary(wall(Cell { -1, 0 }, left), maze));
    REQUIRE_FALSE(is_boundary(wall(Cell { 3, 2 }, right), maze));
}

TEST_CASE("maze internal walls", "[maze]") {
    const Maze maze { 2, 3 };
    REQUIRE(is_internal(wall(Cell { 0, 1 }, up), maze));
    REQUIRE(is_internal(wall(Cell { 1, 1 }, up), maze));
    REQUIRE(is_internal(wall(Cell { 0, 2 }, up), maze));
    REQUIRE(is_internal(wall(Cell { 1, 2 }, up), maze));
    REQUIRE(is_internal(wall(Cell { 1, 0 }, left), maze));
    REQUIRE(is_internal(wall(Cell { 1, 1 }, left), maze));
    REQUIRE(is_internal(wall(Cell { 1, 2 }, left), maze));

    // Boundary.
    REQUIRE_FALSE(is_internal(wall(Cell { 0, 0 }, up), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 1, 0 }, up), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 0, 2 }, down), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 1, 2 }, down), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 0, 0 }, left), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 0, 1 }, left), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 0, 2 }, left), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 1, 0 }, right), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 1, 1 }, right), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 1, 2 }, right), maze));

    // External.
    REQUIRE_FALSE(is_internal(wall(Cell { 0, -1 }, up), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 0, 3 }, down), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { -1, 0 }, left), maze));
    REQUIRE_FALSE(is_internal(wall(Cell { 3, 2 }, right), maze));
}

TEST_CASE("maze cells generator", "[maze]") {
    const Maze maze { 5, 6 };
    const int numCells { 30 };
    const std::unordered_set uniqueReturnedCells(std::from_range, cells(maze));
    const long numUniqueReturnedCells { static_cast<long>(uniqueReturnedCells.size()) };
    const long numReturnedCells { std::ranges::distance(cells(maze)) };

    SECTION("returns as many cells as there are in the maze") { REQUIRE(numReturnedCells == numCells); }
    SECTION("all cells are unique") { REQUIRE(numUniqueReturnedCells == numReturnedCells); }
    SECTION("the maze contains all cells") {
        for (const auto& cell : cells(maze)) {
            REQUIRE(contains(maze, cell));
        }
    }
}

TEST_CASE("cell neighbours generator", "[maze]") {
    // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
    const Maze maze { parse_text("XXXXXXX\n"
                                 "X X   X\n"
                                 "XXX XXX\n"
                                 "X     X\n"
                                 "X X X X\n"
                                 "X     X\n"
                                 "XXXXXXX",
                                 3, 3)
                          .value() };
    const auto& [cell, count] { GENERATE(std::make_pair(Cell { 0, 0 }, 0), std::make_pair(Cell { 2, 0 }, 1),
                                         std::make_pair(Cell { 0, 2 }, 2), std::make_pair(Cell { 1, 2 }, 3),
                                         std::make_pair(Cell { 1, 1 }, 4)) };

    SECTION("returns once for each neighbour") { REQUIRE(std::ranges::distance(neighbours(cell, maze)) == count); }
    SECTION("returns reachable neighbours only") {
        for (const auto& [neighbour, direction] : neighbours(cell, maze)) {
            REQUIRE_FALSE(maze.is_set(wall(cell, direction)));
            REQUIRE(GridMazes::neighbour(cell, direction) == neighbour);
        }
    }
    SECTION("all neighbours unique") {
        auto cellsOnly { neighbours(cell, maze) |
                         std::views::transform([](std::pair<Cell, Direction> pair) { return pair.first; }) };
        REQUIRE(std::unordered_set(std::from_range, cellsOnly).size() == count);
    }
}

TEST_CASE("maze walls generator", "[maze]") {
    const Maze maze { 3, 2 };
    const int numWalls { 17 };
    const std::unordered_set uniqueReturnedWalls(std::from_range, walls(maze));
    const long numUniqueReturnedWalls { static_cast<long>(uniqueReturnedWalls.size()) };
    const long numReturnedWalls { std::ranges::distance(walls(maze)) };

    SECTION("returns as many walls as there are in the maze") { REQUIRE(numReturnedWalls == numWalls); }
    SECTION("all walls are unique") { REQUIRE(numUniqueReturnedWalls == numReturnedWalls); }
    SECTION("the maze contains all walls") {
        for (const auto& wall : walls(maze)) {
            REQUIRE(contains(maze, wall));
        }
    }
}

TEST_CASE("maze internal walls generator", "[maze]") {
    const Maze maze { 4, 15 };
    const int numWalls { 101 };
    const std::unordered_set uniqueReturnedWalls(std::from_range, internal_walls(maze));
    const long numUniqueReturnedWalls { static_cast<long>(uniqueReturnedWalls.size()) };
    const long numReturnedWalls { std::ranges::distance(internal_walls(maze)) };

    SECTION("returns as many internal walls as there are in the maze") { REQUIRE(numReturnedWalls == numWalls); }
    SECTION("all walls are unique") { REQUIRE(numUniqueReturnedWalls == numReturnedWalls); }
    SECTION("all walls are internal") {
        for (const auto& wall : internal_walls(maze)) {
            REQUIRE(is_internal(wall, maze));
        }
    }
}

TEST_CASE("maze boundary walls generator", "[maze]") {
    const Maze maze { 4, 15 };
    const int numWalls { 38 };
    const std::unordered_set uniqueReturnedWalls(std::from_range, boundary_walls(maze));
    const long numUniqueReturnedWalls { static_cast<long>(uniqueReturnedWalls.size()) };
    const long numReturnedWalls { std::ranges::distance(boundary_walls(maze)) };

    SECTION("returns as many boundary walls as there are in the maze") { REQUIRE(numReturnedWalls == numWalls); }
    SECTION("all walls are unique") { REQUIRE(numUniqueReturnedWalls == numReturnedWalls); }
    SECTION("all walls are boundary walls") {
        for (const auto& wall : boundary_walls(maze)) {
            REQUIRE(is_boundary(wall, maze));
        }
    }
}

TEST_CASE("maze is_set", "[maze]") {
    const Maze maze { 3, 2 };
    SECTION("boundary walls are set") {
        for (const auto& wall : boundary_walls(maze)) {
            REQUIRE(maze.is_set(wall));
        }
    }

    SECTION("external walls are unset") {
        REQUIRE_FALSE(maze.is_set(wall(Cell { -1, 0 }, left)));
        REQUIRE_FALSE(maze.is_set(wall(Cell { -1, -1 }, up)));
        REQUIRE_FALSE(maze.is_set(wall(Cell { 3, 2 }, left)));
    }
}

TEST_CASE("maze setters", "[maze]") {
    Maze maze { 3, 2 };

    SECTION("internal walls can be set, unset, and toggled") {
        for (const auto& wall : internal_walls(maze)) {
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

    SECTION("throw when trying to modify a non-internal wall") {
        const Wall boundaryWall { .line = 0, .offset = 0, .orientation = horizontal };
        REQUIRE_THROWS(maze.set(boundaryWall));
        REQUIRE_THROWS(maze.unset(boundaryWall));
        REQUIRE_THROWS(maze.toggle(boundaryWall));

        const Wall externalWall { .line = 0, .offset = -1, .orientation = horizontal };
        REQUIRE_THROWS(maze.set(externalWall));
        REQUIRE_THROWS(maze.unset(externalWall));
        REQUIRE_THROWS(maze.toggle(externalWall));
    }

    SECTION("walls do not affect each other") {
        const Wall myWall { wall(Cell { .x = 0, .y = 0 }, right) };
        maze.set(myWall);
        REQUIRE(maze.is_set(myWall));

        for (const auto& wall : internal_walls(maze)) {
            if (wall != myWall) {
                REQUIRE_FALSE(maze.is_set(wall));
            }
        }
    }
}

TEST_CASE("maze set_all", "[maze]") {
    Maze maze { 15, 4 }; // NOLINT (magic numbers)

    for (const auto& wall : internal_walls(maze)) {
        REQUIRE_FALSE(maze.is_set(wall));
    }
    maze.set_all();
    for (const auto& wall : internal_walls(maze)) {
        REQUIRE(maze.is_set(wall));
    }
}

TEST_CASE("maze unset_all", "[maze]") {
    Maze maze { 15, 4 }; // NOLINT (magic numbers)

    maze.set_all();
    for (const auto& wall : internal_walls(maze)) {
        REQUIRE(maze.is_set(wall));
    }

    maze.unset_all();
    for (const auto& wall : internal_walls(maze)) {
        REQUIRE_FALSE(maze.is_set(wall));
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
    maze.set(wall(Cell { .x = 0, .y = 0 }, right));
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

TEST_CASE("maze formatter", "[maze]") {
    const Maze maze { 40, 2 };
    REQUIRE(std::format("{}", maze) == "maze of width 40 and height 2");
}
