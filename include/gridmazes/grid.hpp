#pragma once
#include <boost/container_hash/hash.hpp>
#include <cstddef>
#include <functional>

namespace GridMazes {
/** Represents an orthogonal direction. */
enum class Direction : std::uint8_t { up, down, left, right };

/** Collection of all orthogonal directions. */
inline constexpr std::array directions { Direction::up, Direction::down, Direction::left, Direction::right };

/** Return the opposite direction. */
[[nodiscard]] constexpr Direction get_opposite(Direction direction) {
    using enum Direction;
    switch (direction) {
    case up:
        return down;
    case down:
        return up;
    case left:
        return right;
    case right:
        return left;
    }
    throw std::runtime_error { "invalid direction" };
}

/** Represents an orientation: horizontal or vertical. */
enum class Orientation : bool { horizontal, vertical };

/** Represents a wall position in a 2D integer grid; that is, the spaces *between* cells.
 *
 * A wall position is defined by a "line", "offset" along that line, and an "orientation".
 *
 * Lines run vertically or horizontally depending on the orientation.
 * The zero lines run touch the cell [0,0] on the left or top.
 *
 * Walls with offset zero are in row/column zero, depending on the orientation.
 * Positive offset is to the right/bottom; negative is to the left/top.
 *
 * Examples:
 * - [line 0, offset 0, horizontal] is the wall up from cell [0,0].
 * - [line 2, offset 2, horizontal] is the wall down from cell [2,1].
 * - [line 0, offset 0, vertical] is the wall left from cell [0,0].
 * - [line 2, offset 2, vertical] is the wall right from cell [1,2]. */
struct Wall {
    /** The number of the row or column the wall is in. */
    int line { 0 };
    /** How far along the wall is along its line. */
    int offset { 0 };
    /** Which way the wall is facing. */
    Orientation orientation { Orientation::horizontal };

    [[nodiscard]] constexpr friend bool operator==(const Wall& lhs, const Wall& rhs) noexcept = default;
};

/** Represents a cell position in a 2D integer grid. */
struct Cell {
    /** Column. */
    int x { 0 };
    /** Row. */
    int y { 0 };

    [[nodiscard]] constexpr friend bool operator==(const Cell& lhs, const Cell& rhs) noexcept = default;

    /** Return the cell `distance` away in `direction` from this one. */
    [[nodiscard]] constexpr Cell translated(Direction direction, int distance = 1) const {
        using enum Direction;
        switch (direction) {
        case up:
            return { .x = x, .y = y - distance };
        case down:
            return { .x = x, .y = y + distance };
        case left:
            return { .x = x - distance, .y = y };
        case right:
            return { .x = x + distance, .y = y };
        }
        throw std::runtime_error { "invalid direction" };
    }

    /** Return the wall in `direction` from this cell. */
    [[nodiscard]] constexpr Wall wall(Direction direction) const {
        using enum Direction;
        using enum Orientation;
        switch (direction) {
        case up:
            return { .line = y, .offset = x, .orientation = horizontal };
        case down:
            return { .line = y + 1, .offset = x, .orientation = horizontal };
        case left:
            return { .line = x, .offset = y, .orientation = vertical };
        case right:
            return { .line = x + 1, .offset = y, .orientation = vertical };
        }
        throw std::runtime_error { "invalid direction" };
    }
};
} // namespace GridMazes

template <>
struct std::hash<GridMazes::Cell> {
    std::size_t operator()(const GridMazes::Cell& cell) const noexcept {
        std::size_t seed { 0 };
        boost::hash_combine(seed, cell.x);
        boost::hash_combine(seed, cell.y);
        return seed;
    }
};

template <>
struct std::hash<GridMazes::Wall> {
    std::size_t operator()(const GridMazes::Wall& wall) const noexcept {
        std::size_t seed { 0 };
        boost::hash_combine(seed, wall.line);
        boost::hash_combine(seed, wall.offset);
        boost::hash_combine(seed, wall.orientation);
        return seed;
    }
};
