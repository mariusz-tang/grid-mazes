#include "gridmazes/maze.hpp"

#include "gridmazes/grid.hpp"

#include <cassert>
#include <cstddef>
#include <format>
#include <generator>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

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

namespace {
/** Get the unique index of an internal wall in a maze. */
[[nodiscard]] int get_internal_wall_index(const Maze& maze, Wall wall) {
    assert(is_internal(wall, maze));

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
    if (!contains(*this, wall)) {
        return false;
    }
    if (is_boundary(wall, *this)) {
        return true;
    }
    return m_internalWalls[get_internal_wall_index(*this, wall)];
}

namespace {
/** Throw an exception if `wall` cannot be set/unset in `maze`. */
void throw_if_immutable(const Maze& maze, Wall wall) {
    if (!is_internal(wall, maze)) {
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

bool contains(const Maze& maze, Cell cell) noexcept {
    const bool has_column { 0 <= cell.x && cell.x < maze.width() };
    const bool has_row { 0 <= cell.y && cell.y < maze.height() };
    return has_column && has_row;
}

bool contains(const Maze& maze, Wall wall) noexcept {
    int max_line {};
    int max_offset {};

    switch (wall.orientation) {
    case Orientation::horizontal:
        max_offset = maze.width() - 1;
        max_line = maze.height();
        break;
    case Orientation::vertical:
        max_offset = maze.height() - 1;
        max_line = maze.width();
        break;
    }
    const bool has_line { 0 <= wall.line && wall.line <= max_line };
    const bool has_offset { 0 <= wall.offset && wall.offset <= max_offset };
    return has_line && has_offset;
}

bool is_boundary(Wall wall, const Maze& maze) noexcept {
    if (!contains(maze, wall)) {
        return false;
    }

    switch (wall.orientation) {
    case Orientation::horizontal:
        return wall.line == 0 || wall.line == maze.height();
    case Orientation::vertical:
        return wall.line == 0 || wall.line == maze.width();
    }
}

bool is_internal(Wall wall, const Maze& maze) noexcept { return contains(maze, wall) && !is_boundary(wall, maze); }

namespace {
/** Return the cells of a maze with shape `width` by `height`. */
std::generator<Cell> maze_cells(int width, int height) noexcept {
    for (int column { 0 }; column < width; column++) {
        for (int row { 0 }; row < height; row++) {
            co_yield { .x = column, .y = row };
        }
    }
}
} // namespace

std::generator<Cell> cells(const Maze& maze) noexcept { return maze_cells(maze.width(), maze.height()); }

std::vector<std::pair<Cell, Direction>> neighbours(Cell cell, const Maze& maze) {
    std::vector<std::pair<Cell, Direction>> result {};
    for (const auto& direction : directions) {
        if (!maze.is_set(wall(cell, direction))) {
            result.emplace_back(neighbour(cell, direction), direction);
        }
    }
    return result;
}

namespace {
/** Return the internal walls for a `width` by `height` maze. */
std::generator<Wall> internal_walls(int width, int height) noexcept {
    for (int line { 1 }; line < height; line++) {
        for (int offset { 0 }; offset < width; offset++) {
            co_yield { .line = line, .offset = offset, .orientation = Orientation::horizontal };
        }
    }
    for (int line { 1 }; line < width; line++) {
        for (int offset { 0 }; offset < height; offset++) {
            co_yield { .line = line, .offset = offset, .orientation = Orientation::vertical };
        }
    }
}

/** Return the boundary walls for a `width` by `height` maze. */
std::generator<Wall> boundary_walls(int width, int height) noexcept {
    for (int offset { 0 }; offset < width; offset++) {
        co_yield { .line = 0, .offset = offset, .orientation = Orientation::horizontal };
        co_yield { .line = height, .offset = offset, .orientation = Orientation::horizontal };
    }
    for (int offset { 0 }; offset < height; offset++) {
        co_yield { .line = 0, .offset = offset, .orientation = Orientation::vertical };
        co_yield { .line = width, .offset = offset, .orientation = Orientation::vertical };
    }
}

/** Return the internal and boundary walls for a `width` by `height` maze. */
std::generator<Wall> walls(int width, int height) noexcept {
    co_yield std::ranges::elements_of(internal_walls(width, height));
    co_yield std::ranges::elements_of(boundary_walls(width, height));
}
} // namespace

std::generator<Wall> walls(const Maze& maze) noexcept { return walls(maze.width(), maze.height()); }

std::generator<Wall> internal_walls(const Maze& maze) noexcept { return internal_walls(maze.width(), maze.height()); }

std::generator<Wall> boundary_walls(const Maze& maze) noexcept { return boundary_walls(maze.width(), maze.height()); }
} // namespace GridMazes
