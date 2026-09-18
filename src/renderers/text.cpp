#include "gridmazes/renderers/text.hpp"

#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"

#include <cassert>
#include <cstddef>
#include <format>
#include <optional>
#include <regex>
#include <string>
#include <string_view>

namespace GridMazes {
namespace {
constexpr char wallMarker = 'X';
constexpr char emptyMarker = ' ';

/** Create an alternating row with `num_cells` empty spaces. */
std::string make_alternating_row(int numCells) {
    std::string row { wallMarker };
    for (int i { 0 }; i < numCells; i++) {
        row += emptyMarker;
        row += wallMarker;
    }
    return row;
}

/** Return the width of a frame for a maze of the given width.
 *
 * Excludes the newlines at the end of each row. */
int frame_width(int mazeWidth) {
    // A wall and an empty for each cell, plus the right/bottom-most walls.
    return (2 * mazeWidth) + 1;
}

/** Create an empty frame with no internal walls. */
std::string make_frame(int width, int height) {
    assert(width > 0);
    assert(height > 0);

    // A wall and an empty for each cell, plus the right/bottom-most walls.
    const int frameWidth { frame_width(width) };
    const std::string rowFull(frameWidth, wallMarker);
    const std::string rowEmpty { wallMarker + std::string(frameWidth - 2, emptyMarker) + wallMarker };
    const std::string rowAlternating { make_alternating_row(width) };

    // Top boundary and first row cells.
    std::string frame { rowFull + '\n' + rowEmpty + '\n' };

    // Internal corners and cells.
    for (int i { 0 }; i < height - 1; i++) {
        frame += rowAlternating + '\n';
        frame += rowEmpty + '\n';
    }

    // Bottom boundary.
    frame += rowFull;

    return frame;
}

/** Return the index of the wall in a maze string with the given width. */
std::size_t get_index(int frameWidth, Wall wall) {
    // Every second row/column is a corner.
    int column {};
    int row {};

    switch (wall.orientation) {
    case Orientation::horizontal:
        column = (2 * wall.offset) + 1;
        row = (2 * wall.line);
        break;
    case Orientation::vertical:
        column = (2 * wall.line);
        row = (2 * wall.offset) + 1;
        break;
    }

    // Cast to prevent widening conversion.
    return (static_cast<long>(row * (frameWidth + 1))) + column;
}
} // namespace

std::string to_text(const Maze& maze) {
    // Start with an empty frame.
    std::string result { make_frame(maze.width(), maze.height()) };

    // Insert walls where present.
    const auto frameWidth { frame_width(maze.width()) };
    for (const auto& wall : internal_walls(maze)) {
        if (maze.is_set(wall)) {
            result[get_index(frameWidth, wall)] = wallMarker;
        }
    }
    return result;
}

namespace {
/** Return true if `text` is a valid text representation of a maze of dimensions `width` by `height`. */
[[nodiscard]] bool is_valid(std::string_view text, int width, int height) {
    const int frameWidth { frame_width(width) };

    const std::string boundaryRegex { std::format("X{{{}}}", frameWidth) };
    const std::string evenRowRegex { std::format("X( (X| )){{{}}} X", width - 1) };
    const std::string oddRowRegex { std::format("X((X| )X){{{}}}", width) };
    const std::regex regex { std::format("{0}\n({1}\n{2}\n){{{3}}}{1}\n{0}", boundaryRegex, evenRowRegex, oddRowRegex,
                                         height - 1) };

    return std::regex_match(text.cbegin(), text.cend(), regex);
}
} // namespace

std::optional<Maze> parse_text(std::string_view text, int width, int height) {
    if (!is_valid(text, width, height)) {
        return {};
    }

    Maze result { width, height };
    const int frameWidth { frame_width(width) };
    // Set every wall which is present.
    for (const auto& wall : internal_walls(result)) {
        if (text[get_index(frameWidth, wall)] == wallMarker) {
            result.set(wall);
        }
    }
    return result;
}

} // namespace GridMazes
