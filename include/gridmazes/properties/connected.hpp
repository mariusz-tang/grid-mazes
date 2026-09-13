#include "gridmazes/maze.hpp"

namespace GridMazes {
/** Return true if the maze is globally connected.
 *
 * A maze is "globally connected" if every pair of contained cells is connected. */
[[nodiscard]] bool is_connected(const Maze& maze);

/** Return true if `start` and `finish` are connected in `maze`.
 *
 * Two cells in a maze are "connected" if there is a path between them that
 * does not cross any impassable walls. */
[[nodiscard]] bool is_connected(const Maze& maze, Cell start, Cell finish);
} // namespace GridMazes
