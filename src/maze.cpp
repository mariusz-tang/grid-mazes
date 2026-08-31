#include "gridmazes/maze.hpp"

#include "gridmazes/position.hpp"

#include <cassert>
#include <cstddef>
#include <format>
#include <generator>
#include <stdexcept>

namespace GridMazes {

using enum Direction;

Maze::Maze(int width, int height)
    : m_width { width }, m_height { height },
      m_internal_walls { static_cast<std::size_t>((2 * width * height) - width - height) } {
    if (m_width <= 0 || m_height <= 0) {
        throw std::runtime_error { std::format("width and height must be strictly positive, but "
                                               "received {} and {}",
                                               m_width, m_height) };
    }
}

bool Maze::contains_position(Position position) const noexcept {
    const auto has_column { 0 <= position.column && position.column < m_width };
    const auto has_row { 0 <= position.row && position.row < m_height };
    return has_column && has_row;
}

bool Maze::has_wall(Position position, Direction direction) const {
    if (!contains_position(position)) {
        throw std::runtime_error { std::format("maze with width {} and height {} does not contain position {}", m_width,
                                               m_height, position) };
    }

    // External walls are always present.
    if (is_external_wall(position, direction)) {
        return true;
    }
    return m_internal_walls[get_wall_index(position, direction)];
}

void Maze::set_wall(Position position, Direction direction, bool state) {
    if (!contains_position(position)) {
        throw std::runtime_error { std::format("maze with width {} and height {} does not contain position {}", m_width,
                                               m_height, position) };
    }

    // External walls are always present.
    if (is_external_wall(position, direction)) {
        throw std::runtime_error { std::format("tried to set an external wall", m_width, m_height, position) };
    }
    m_internal_walls[get_wall_index(position, direction)] = state;
}

bool Maze::is_external_wall(Position position, Direction direction) const {
    if (!contains_position(position)) {
        throw std::runtime_error { std::format("maze with width {} and height {} does not contain position {}", m_width,
                                               m_height, position) };
    }

    switch (direction) {
    case up:
        return position.row == 0;
    case down:
        return position.row == m_height - 1;
    case left:
        return position.column == 0;
    case right:
        return position.column == m_width - 1;
    }
    throw std::runtime_error { "invalid direction" };
}

/** Get the index in m_walls corresponding to this wall. */
int Maze::get_wall_index(Position position, Direction direction) const {
    assert(contains_position(position));
    assert(is_internal_wall(position, direction));

    // Normalize to face up or left.
    if (direction == down || direction == right) {
        position = position.get_neighbour(direction);
        direction = get_opposite(direction);
    }

    // Horizontal walls above cells in the first column.
    if (position.column == 0 && direction == up) {
        return position.row - 1;
    }

    // Vertical walls to the left of cells in the first row.
    if (position.row == 0 && direction == left) {
        return m_height - 1 + position.column - 1;
    }

    // Walls above or to the left of the other cells in reading order.
    const auto offset { m_width + m_height - 2 };
    const auto position_index { position.column - 1 + ((m_width - 1) * (position.row - 1)) };
    if (direction == up) {
        return offset + (2 * position_index);
    }
    return offset + (2 * position_index) + 1;
}

std::generator<Position> Maze::positions() const noexcept {
    for (int column { 0 }; column < m_width; column++) {
        for (int row { 0 }; row < m_height; row++) {
            co_yield { .column = column, .row = row };
        }
    }
}
} // namespace GridMazes
