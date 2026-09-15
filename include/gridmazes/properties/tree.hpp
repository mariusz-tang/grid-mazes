#include "gridmazes/maze.hpp"

namespace GridMazes {
/** Return true if the maze is fully connected and has no loops. */
[[nodiscard]] bool is_tree(const Maze& maze);
} // namespace GridMazes
