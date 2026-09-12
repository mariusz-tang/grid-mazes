#pragma once
#include "gridmazes/maze.hpp"

#include <optional>
#include <string>
#include <string_view>

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

/** Construct a maze from an input string and return it, or return empty if the input is invalid.
 *
 * Valid input strings are in the same form as the output of `to_text(const Maze&)`
 * for an input maze of the same `width` and `height`. */
[[nodiscard]] std::optional<Maze> parse_text(std::string_view text, int width, int height);
} // namespace GridMazes
