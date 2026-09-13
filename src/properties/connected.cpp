#include "gridmazes/properties/connected.hpp"

#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"

#include <cstddef>
#include <stack>
#include <unordered_set>

namespace GridMazes {
namespace {
/** Return the set of cells connected to and including `cell` in `maze`. */
[[nodiscard]] std::unordered_set<Cell> connected_cells(const Maze& maze, Cell cell) {
    std::unordered_set<Cell> visitedCells { cell };
    std::stack<Cell> newCells { { cell } };

    // Keep exploring neighbours until no new cells are found.
    while (!newCells.empty()) {
        // Grab a new cell and remove it from the stack.
        const Cell cell { newCells.top() };
        newCells.pop();

        // Add its unvisited neighbours to the visited cells and the stack.
        for (const auto& [neighbour, _] : maze.neighbours(cell)) {
            if (!visitedCells.contains(neighbour)) {
                visitedCells.insert(neighbour);
                newCells.push(neighbour);
            }
        }
    }
    return visitedCells;
}

} // namespace

bool is_connected(const Maze& maze) {
    // The maze is globally connected if every cell is connected to [0,0].
    const auto numCells { static_cast<size_t>(maze.width() * maze.height()) };
    const auto numConnectedCells { connected_cells(maze, Cell { .x = 0, .y = 0 }).size() };
    return numConnectedCells == numCells;
}

bool is_connected(const Maze& maze, Cell start, Cell finish) { return connected_cells(maze, start).contains(finish); }
} // namespace GridMazes
