#include "gridmazes/maze.hpp"

#include "gridmazes/grid.hpp"

#include <cassert>
#include <cstddef>
#include <format>
#include <generator>
#include <stdexcept>

namespace GridMazes {
using enum Direction;

namespace {
/** Return the number of internal walls in a `width` by `height` maze. */
[[nodiscard]] std::size_t num_internal_walls(int width, int height) { return (2 * width * height) - width - height; }
} // namespace

Maze::Maze(int width, int height)
    : m_width { width }, m_height { height }, m_internalWalls { num_internal_walls(width, height) } {
    if (m_width <= 0 || m_height <= 0) {
        throw std::runtime_error { std::format("width and height must be strictly positive, but "
                                               "received {} and {}",
                                               m_width, m_height) };
    }
}

bool Maze::contains(Cell cell) const noexcept {
    const bool has_column { 0 <= cell.x && cell.x < m_width };
    const bool has_row { 0 <= cell.y && cell.y < m_height };
    return has_column && has_row;
}

bool Maze::contains(Wall wall) const noexcept {
    int max_line {};
    int max_offset {};

    switch (wall.orientation) {
    case Orientation::horizontal:
        max_offset = m_width - 1;
        max_line = m_height;
        break;
    case Orientation::vertical:
        max_offset = m_height - 1;
        max_line = m_width;
        break;
    }
    const bool has_line { 0 <= wall.line && wall.line <= max_line };
    const bool has_offset { 0 <= wall.offset && wall.offset <= max_offset };
    return has_line && has_offset;
}

bool Maze::is_boundary(Wall wall) const noexcept {
    if (!contains(wall)) {
        return false;
    }

    switch (wall.orientation) {
    case Orientation::horizontal:
        return wall.line == 0 || wall.line == height();
    case Orientation::vertical:
        return wall.line == 0 || wall.line == width();
    }
}

bool Maze::is_internal(Wall wall) const noexcept { return contains(wall) && !is_boundary(wall); }

std::generator<Cell> Maze::cells() const noexcept {
    for (int column { 0 }; column < m_width; column++) {
        for (int row { 0 }; row < m_height; row++) {
            co_yield { .x = column, .y = row };
        }
    }
}

std::generator<Wall> Maze::walls() const noexcept {
    for (int line { 0 }; line <= m_height; line++) {
        for (int offset { 0 }; offset < m_width; offset++) {
            co_yield { .line = line, .offset = offset, .orientation = Orientation::horizontal };
        }
    }
    for (int line { 0 }; line <= m_width; line++) {
        for (int offset { 0 }; offset < m_height; offset++) {
            co_yield { .line = line, .offset = offset, .orientation = Orientation::vertical };
        }
    }
}

std::generator<Wall> Maze::internal_walls() const noexcept {
    for (const auto& wall : walls()) {
        if (is_internal(wall)) {
            co_yield wall;
        }
    }
}

std::generator<Wall> Maze::boundary_walls() const noexcept {
    for (const auto& wall : walls()) {
        if (is_boundary(wall)) {
            co_yield wall;
        }
    }
}

namespace {
/** Get the unique index of an internal wall in a maze. */
[[nodiscard]] int get_internal_wall_index(const Maze& maze, Wall wall) {
    assert(maze.is_internal(wall));

    switch (wall.orientation) {
    case Orientation::horizontal:
        // Horizontal walls in reading order...
        return ((wall.line - 1) * maze.width()) + wall.offset;
    case Orientation::vertical:
        // ...followed by vertical walls in column order.
        const int numHorizontalWalls { maze.width() * (maze.height() - 1) };
        return numHorizontalWalls + ((wall.line - 1) * maze.height()) + wall.offset;
    }
}
} // namespace

[[nodiscard]] bool Maze::is_set(Wall wall) const noexcept {
    if (!contains(wall)) {
        return false;
    }
    if (is_boundary(wall)) {
        return true;
    }
    return m_internalWalls[get_internal_wall_index(*this, wall)];
}

namespace {
/** Throw an exception if `wall` cannot be set/unset in `maze`. */
void throw_if_immutable(const Maze& maze, Wall wall) {
    if (!maze.is_internal(wall)) {
        throw std::runtime_error("tried to set or unset a non-internal wall");
    }
}
} // namespace

void Maze::set(Wall wall) {
    throw_if_immutable(*this, wall);
    m_internalWalls[get_internal_wall_index(*this, wall)] = true;
}

void Maze::unset(Wall wall) {
    throw_if_immutable(*this, wall);
    m_internalWalls[get_internal_wall_index(*this, wall)] = false;
}

void Maze::toggle(Wall wall) {
    throw_if_immutable(*this, wall);
    if (is_set(wall)) {
        unset(wall);
    } else {
        set(wall);
    }
}
} // namespace GridMazes
