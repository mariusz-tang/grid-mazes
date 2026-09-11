#pragma once
#include "gridmazes/maze.hpp"

#include <string>

namespace GridMazes {
/** Return a string representing an image of a maze.
 *
 * For example, given a 3x4 maze, the result could be:
 * XXXXXXX
 * X     X
 * XXX XXX
 * X   X X
 * X XXX X
 * X X   X
 * X X X X
 * X   X X
 * XXXXXXX
 *
 * Newlines are placed inside the string but not at the end. */
[[nodiscard]] std::string to_text(const Maze& maze);
} // namespace GridMazes
