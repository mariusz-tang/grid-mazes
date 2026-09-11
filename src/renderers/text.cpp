#include "gridmazes/renderers/text.hpp"

#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"

#include <cassert>
#include <cstddef>
#include <string>

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
std::size_t get_index(int frameWidth, const Wall& wall) {
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
    for (const auto& wall : maze.walls()) {
        if (maze.is_internal(wall) && maze.is_set(wall)) {
            result[get_index(frameWidth, wall)] = wallMarker;
        }
    }
    return result;
}
} // namespace GridMazes
