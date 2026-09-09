#include "gridmazes/generators/kruskal.hpp"

#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"

#include <cassert>
#include <generator>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace GridMazes::Generators {

namespace {
using CellGroup = std::unordered_set<Cell>;
using CellGroupMap = std::unordered_map<Cell, std::shared_ptr<CellGroup>>;

/** Return one group for each cell, containing that cell only. */
[[nodiscard]] CellGroupMap make_cell_groups(const Maze& maze) {
    CellGroupMap groups {};
    for (const auto& cell : maze.cells()) {
        groups[cell] = std::make_shared<CellGroup>();
        groups[cell]->insert(cell);
    }
    return groups;
}

/** Return true if the pair of cells are in the same group. */
bool in_same_group(const CellGroupMap& groups_by_cell, const std::pair<Cell, Cell>& cells) {
    return groups_by_cell.at(cells.first)->contains(cells.second);
}

/** Join the groups containing two cells. */
void merge_cell_groups(CellGroupMap& groups_by_cell, const std::pair<Cell, Cell>& cells) {
    const std::shared_ptr<CellGroup> group_a { groups_by_cell[cells.first] };
    const std::shared_ptr<CellGroup> group_b { groups_by_cell[cells.second] };

    assert(group_a);
    assert(group_b);

    // Change all the cells in B to point at A instead.
    for (const auto& cell : *group_b) {
        groups_by_cell[cell] = group_a;
    }
    // Move all the elements of B into A.
    group_a->merge(*group_b);
}
} // namespace

namespace Private {

/** Generate a maze in `maze` using Kruskal's algorithm and return a reference to the maze at each step.
 *
 * `orderedWalls` determines the order in which the walls are checked. */
std::generator<const Maze&> kruskal_steps(Maze maze, std::vector<Wall> orderedWalls) {
    CellGroupMap groups_by_cell { make_cell_groups(maze) };

    // Start with all walls impassable.
    maze.set_all();
    co_yield maze;

    for (const auto& wall : orderedWalls) {
        assert(maze.contains(wall));
        if (maze.is_boundary(wall)) {
            continue;
        }

        // If the wall separates two different groups...
        if (!in_same_group(groups_by_cell, wall.cells())) {
            // ...then make it passable and merge the groups.
            maze.unset(wall);
            merge_cell_groups(groups_by_cell, wall.cells());
            co_yield maze;
        }
    }
}

/** Generate a maze in `maze` using Kruskal's algorithm.
 *
 * `orderedWalls` determines the order in which the walls are checked. */
Maze kruskal(Maze& maze, const std::vector<Wall>& orderedWalls) {
    CellGroupMap groups_by_cell { make_cell_groups(maze) };

    // Start with all walls impassable.
    maze.set_all();

    for (const auto& wall : orderedWalls) {
        assert(maze.contains(wall));
        if (maze.is_boundary(wall)) {
            continue;
        }

        // If the wall separates two different groups...
        if (!in_same_group(groups_by_cell, wall.cells())) {
            // ...then make it passable and merge the groups.
            maze.unset(wall);
            merge_cell_groups(groups_by_cell, wall.cells());
        }
    }
    return maze;
}
} // namespace Private
} // namespace GridMazes::Generators
