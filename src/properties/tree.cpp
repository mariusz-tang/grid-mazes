#include "gridmazes/properties/tree.hpp"

#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"

#include <stack>
#include <unordered_set>

namespace GridMazes {

bool is_tree(const Maze& maze) {
    std::unordered_set<Cell> visitedCells { Cell { .x = 0, .y = 0 } };
    std::stack<Cell> newCells { { Cell { .x = 0, .y = 0 } } };

    // Keep exploring neighbours until no new cells are found.
    while (!newCells.empty()) {
        // Grab a new cell and remove it from the stack.
        const Cell cell { newCells.top() };
        newCells.pop();

        int visitedNeighbours { 0 };
        for (const auto& [neighbour, _] : maze.neighbours(cell)) {
            if (visitedCells.contains(neighbour)) {
                // Keep track of the number of neighbours that have already been visited.
                visitedNeighbours++;
            } else {
                // Add its unvisited neighbours to the visited cells and the stack.
                visitedCells.insert(neighbour);
                newCells.push(neighbour);
            }
        }
        // All but the first cell will have at least one visited neighbour (the
        // cell that came before). If any cell has more than one visited neighbour
        // then we have visited the other neighbours for the second time and the
        // maze has a cycle, so it is not a tree. This cannot happen on the first
        // cell so there is no special case required.
        if (visitedNeighbours >= 2) {
            return false;
        }
    }

    // If there were no cycles then the maze is a tree if it is fully connected,
    // i.e. if we visited every cell.
    return visitedCells.size() == maze.size();
}
} // namespace GridMazes
