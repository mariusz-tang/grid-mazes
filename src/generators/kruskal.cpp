#include "gridmazes/generators/kruskal.hpp"

#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"

#include <cassert>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

namespace GridMazes {

namespace Private {
/** Place each cell into its own group. */
[[nodiscard]] CellGroupMap make_cell_groups(const Maze& maze) {
    CellGroupMap groups {};
    for (const auto& cell : cells(maze)) {
        groups[cell] = std::make_shared<CellGroup>();
        groups[cell]->insert(cell);
    }
    return groups;
}
} // namespace Private

namespace {
using Private::CellGroup;
using Private::CellGroupMap;

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

void KruskalGenerator::step_impl() {
    assert(m_position < m_walls.size());
    assert(!in_same_group(m_groups_by_cell, neighbours(m_walls[m_position])));

    // Remove the next wall.
    const Wall wall { m_walls[m_position] };
    m_maze.unset(wall);
    merge_cell_groups(m_groups_by_cell, neighbours(wall));

    // Advance until we reach a wall which separates two different groups or we run out of walls.
    m_position++;
    while (m_position < m_walls.size() && in_same_group(m_groups_by_cell, neighbours(m_walls[m_position]))) {
        m_position++;
    }

    // If we ran out of walls then we are done.
    if (m_position == m_walls.size()) {
        m_terminated = true;
    }
}

} // namespace GridMazes
