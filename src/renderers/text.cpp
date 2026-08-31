#include "gridmazes/renderers/text.hpp"

#include "gridmazes/maze.hpp"
#include "gridmazes/position.hpp"

#include <cassert>
#include <cstddef>
#include <string>

namespace GridMazes::Render {
namespace {
constexpr char wall = 'X';
constexpr char empty = ' ';

/** Create an alternating row with `num_cells` empty spaces. */
std::string make_alternating_row(int numCells) {
    std::string row { wall };
    for (int i { 0 }; i < numCells; i++) {
        row += empty;
        row += wall;
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
    const std::string rowFull(frameWidth, wall);
    const std::string rowEmpty { wall + std::string(frameWidth - 2, empty) + wall };
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

/** Return the index of the wall in `direction` from `position`. */
std::size_t get_index(int frameWidth, Position position, Direction direction) {
    assert(direction == Direction::right || direction == Direction::down);

    // Every second row/column is a corner.
    int column { (2 * position.column) + 1 };
    int row { (2 * position.row) + 1 };

    if (direction == Direction::right) {
        column++;
    } else if (direction == Direction::down) {
        row++;
    }

    // Prevent widening conversion.
    return (static_cast<long>(row * (frameWidth + 1))) + column;
}

} // namespace

std::string to_text(const Maze& maze) {
    // Start with an empty frame.
    std::string result { make_frame(maze.width(), maze.height()) };

    // Insert walls where present.
    const auto frameWidth { frame_width(maze.width()) };
    for (const auto& position : maze.positions()) {
        if (maze.has_wall(position, Direction::right)) {
            result[get_index(frameWidth, position, Direction::right)] = wall;
        }
        if (maze.has_wall(position, Direction::down)) {
            result[get_index(frameWidth, position, Direction::down)] = wall;
        }
    }
    return result;
}
} // namespace GridMazes::Render
