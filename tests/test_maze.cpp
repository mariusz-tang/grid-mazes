#include "gridmazes/maze.hpp"
#include "gridmazes/position.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>

TEST_CASE("maze width and height", "[maze]") {
    const auto width { GENERATE(range(1, 10)) };
    const auto height { GENERATE(range(1, 10)) };
    const GridMazes::Maze maze { width, height };

    REQUIRE(maze.height() == height);
    REQUIRE(maze.width() == width);
}

TEST_CASE("maze constructor throws if either dimension is non-positive", "[maze]") {
    const auto bad_dimension_0 { GENERATE(range(-3, 1)) };
    const auto bad_dimension_1 { GENERATE(range(-3, 1)) };
    REQUIRE_THROWS(GridMazes::Maze { 1, bad_dimension_0 });
    REQUIRE_THROWS(GridMazes::Maze { bad_dimension_0, 1 });
    REQUIRE_THROWS(GridMazes::Maze { bad_dimension_0, bad_dimension_1 });
}

TEST_CASE("maze internal cells", "[maze]") {
    const auto width { 11 };
    const auto height { 5 };
    GridMazes::Maze maze { width, height };

    const auto column { GENERATE_COPY(range(0, width)) };
    const auto row { GENERATE_COPY(range(0, height)) };
    const GridMazes::Position position { .column = column, .row = row };

    const auto direction { GENERATE(from_range(GridMazes::directions)) };

    SECTION("contains_position returns true") { REQUIRE(maze.contains_position(position)); }

    SECTION("internal and external walls") {
        if ((column == 0 && direction == GridMazes::Direction::left) ||
            (column == maze.width() - 1 && direction == GridMazes::Direction::right) ||
            (row == 0 && direction == GridMazes::Direction::up) ||
            (row == maze.height() - 1 && direction == GridMazes::Direction::down)) {
            REQUIRE(maze.is_external_wall(position, direction));
            REQUIRE_FALSE(maze.is_internal_wall(position, direction));
        } else {
            REQUIRE(maze.is_internal_wall(position, direction));
            REQUIRE_FALSE(maze.is_external_wall(position, direction));
        }
    }

    SECTION("internal walls are all absent and external all present after"
            " construction ") {
        const auto has_wall { maze.has_wall({ .column = column, .row = row }, direction) };
        if (maze.is_external_wall({ .column = column, .row = row }, direction)) {
            REQUIRE(has_wall);
        } else {
            REQUIRE_FALSE(has_wall);
        }
    }

    SECTION("walls have the same value from both sides") {
        if (maze.is_internal_wall(position, direction)) {
            maze.place_wall(position, direction);

            const auto opposite_position { position.get_neighbour(direction) };
            const auto opposite_direction { GridMazes::get_opposite(direction) };

            REQUIRE(maze.has_wall(position, direction));
            REQUIRE(maze.has_wall(opposite_position, opposite_direction));

            maze.remove_wall(position, direction);
            REQUIRE_FALSE(maze.has_wall(position, direction));
            REQUIRE_FALSE(maze.has_wall(opposite_position, opposite_direction));
        }
    }

    SECTION("toggle_wall") {
        if (maze.is_internal_wall(position, direction)) {
            maze.toggle_wall(position, direction);
            REQUIRE(maze.has_wall(position, direction));
            maze.toggle_wall(position, direction);
            REQUIRE_FALSE(maze.has_wall(position, direction));
        }
    }

    SECTION("wall setters throw on external walls") {
        if (maze.is_external_wall(position, direction)) {
            REQUIRE_THROWS(maze.place_wall(position, direction));
            REQUIRE_THROWS(maze.remove_wall(position, direction));
            REQUIRE_THROWS(maze.toggle_wall(position, direction));
        }
    }

    SECTION("walls do not affect other walls") {
        const auto wall_column { GENERATE_COPY(range(0, width)) };
        const auto wall_row { GENERATE_COPY(range(0, height)) };

        // Pick a TEST wall.
        const GridMazes::Position wall_position { .column = wall_column, .row = wall_row };
        const auto wall_direction { GENERATE(GridMazes::Direction::down, GridMazes::Direction::right) };

        // Store the TEST wall from the opposite cell as well.
        const auto opposite_position { wall_position.get_neighbour(wall_direction) };
        const auto opposite_direction { GridMazes::get_opposite(wall_direction) };

        // If both walls are internal.
        if (maze.is_internal_wall(position, direction) && maze.is_internal_wall(wall_position, wall_direction)) {
            // Set the TEST wall.
            maze.place_wall(wall_position, wall_direction);
            REQUIRE(maze.has_wall(wall_position, wall_direction));

            // If the current wall is different from the TEST wall...
            const bool different_wall { (position != wall_position || direction != wall_direction) &&
                                        (position != opposite_position || direction != opposite_direction) };
            if (different_wall) {
                // ...then it should still be unset.
                REQUIRE_FALSE(maze.has_wall(position, direction));
            }
        }
    }
}

TEST_CASE("maze external cells", "[maze]") {
    const GridMazes::Maze maze { 5, 8 };
    const auto position { GENERATE(GridMazes::Position { 0, -1 }, GridMazes::Position { -1, 0 },
                                   GridMazes::Position { 5, 0 }, GridMazes::Position { 0, 8 },
                                   GridMazes::Position { -1, -1 }, GridMazes::Position { 5, 8 },
                                   GridMazes::Position { -2, 4 }, GridMazes::Position { 4, 10 },
                                   GridMazes::Position { 4, 9 }, GridMazes::Position { 6, 4 }) };

    SECTION("contains_position returns false") { REQUIRE_FALSE(maze.contains_position(position)); }

    SECTION("wall functions throw") {
        for (const auto direction : GridMazes::directions) {
            REQUIRE_THROWS(maze.is_internal_wall(position, direction));
            REQUIRE_THROWS(maze.is_external_wall(position, direction));
            REQUIRE_THROWS(maze.has_wall(position, direction));
        }
    }
}

TEST_CASE("maze clear", "[maze]") {
    const auto width { 4 };
    const auto height { 3 };
    GridMazes::Maze maze { width, height };
    maze.fill();
    maze.clear();
    REQUIRE_FALSE(maze.has_wall({ 0, 0 }, GridMazes::Direction::right));
    REQUIRE_FALSE(maze.has_wall({ 0, 1 }, GridMazes::Direction::right));
    REQUIRE_FALSE(maze.has_wall({ 0, 2 }, GridMazes::Direction::right));
    REQUIRE_FALSE(maze.has_wall({ 1, 0 }, GridMazes::Direction::right));
    REQUIRE_FALSE(maze.has_wall({ 1, 1 }, GridMazes::Direction::right));
    REQUIRE_FALSE(maze.has_wall({ 1, 2 }, GridMazes::Direction::right));
    REQUIRE_FALSE(maze.has_wall({ 2, 0 }, GridMazes::Direction::right));
    REQUIRE_FALSE(maze.has_wall({ 2, 1 }, GridMazes::Direction::right));
    REQUIRE_FALSE(maze.has_wall({ 2, 2 }, GridMazes::Direction::right));
    REQUIRE_FALSE(maze.has_wall({ 0, 0 }, GridMazes::Direction::down));
    REQUIRE_FALSE(maze.has_wall({ 0, 1 }, GridMazes::Direction::down));
    REQUIRE_FALSE(maze.has_wall({ 1, 0 }, GridMazes::Direction::down));
    REQUIRE_FALSE(maze.has_wall({ 1, 1 }, GridMazes::Direction::down));
    REQUIRE_FALSE(maze.has_wall({ 2, 0 }, GridMazes::Direction::down));
    REQUIRE_FALSE(maze.has_wall({ 2, 1 }, GridMazes::Direction::down));
    REQUIRE_FALSE(maze.has_wall({ 3, 0 }, GridMazes::Direction::down));
    REQUIRE_FALSE(maze.has_wall({ 3, 1 }, GridMazes::Direction::down));
}

TEST_CASE("maze is_empty", "[maze]") {
    const auto width { 4 };
    const auto height { 2 };
    GridMazes::Maze maze { width, height };

    SECTION("maze is empty on construction") { REQUIRE(maze.is_empty()); }
    SECTION("adding a wall makes it non-empty and removing it makes it empty again") {
        maze.place_wall({ .column = 2, .row = 1 }, GridMazes::Direction::right);
        REQUIRE_FALSE(maze.is_empty());
        maze.remove_wall({ .column = 2, .row = 1 }, GridMazes::Direction::right);
        REQUIRE(maze.is_empty());
    }
    SECTION("clearing a maze makes it empty") {
        maze.place_wall({ .column = 2, .row = 1 }, GridMazes::Direction::right);
        maze.place_wall({ .column = 1, .row = 1 }, GridMazes::Direction::up);
        maze.place_wall({ .column = 0, .row = 1 }, GridMazes::Direction::right);
        REQUIRE_FALSE(maze.is_empty());
        maze.clear();
        REQUIRE(maze.is_empty());
    }
}

TEST_CASE("maze fill", "[maze]") {
    const auto width { 2 };
    const auto height { 3 };
    GridMazes::Maze maze { width, height };
    maze.fill();
    REQUIRE(maze.has_wall({ 0, 0 }, GridMazes::Direction::right));
    REQUIRE(maze.has_wall({ 0, 1 }, GridMazes::Direction::right));
    REQUIRE(maze.has_wall({ 0, 2 }, GridMazes::Direction::right));
    REQUIRE(maze.has_wall({ 0, 0 }, GridMazes::Direction::down));
    REQUIRE(maze.has_wall({ 0, 1 }, GridMazes::Direction::down));
    REQUIRE(maze.has_wall({ 1, 0 }, GridMazes::Direction::down));
    REQUIRE(maze.has_wall({ 1, 1 }, GridMazes::Direction::down));
}

TEST_CASE("maze is_full", "[maze]") {
    const auto width { 3 };
    const auto height { 3 };
    GridMazes::Maze maze { width, height };

    SECTION("maze is not full on construction") { REQUIRE_FALSE(maze.is_full()); }

    SECTION("filling the maze makes it full and then removing a wall makes it not full") {
        maze.fill();
        REQUIRE(maze.is_full());
        maze.remove_wall({ .column = 1, .row = 1 }, GridMazes::Direction::right);
        REQUIRE_FALSE(maze.is_full());
    }
}
