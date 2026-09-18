#pragma once
#include <boost/container_hash/hash.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace GridMazes {
/** Represents an orthogonal direction. */
enum class Direction : std::uint8_t { up, down, left, right };

/** Collection of all orthogonal directions. */
inline constexpr std::array directions { Direction::up, Direction::down, Direction::left, Direction::right };

/** Return the opposite direction. */
[[nodiscard]] constexpr Direction opposite(Direction direction) {
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

/** Represents a cell position in a 2D integer grid. */
struct Cell {
    /** Column. */
    int x { 0 };
    /** Row. */
    int y { 0 };

    [[nodiscard]] constexpr bool operator==(this Cell, Cell) noexcept = default;
};

/** Return the next cell in `direction` from `cell`. */
[[nodiscard]] constexpr Cell neighbour(Cell cell, Direction direction) {
    using enum Direction;
    switch (direction) {
    case up:
        return { .x = cell.x, .y = cell.y - 1 };
    case down:
        return { .x = cell.x, .y = cell.y + 1 };
    case left:
        return { .x = cell.x - 1, .y = cell.y };
    case right:
        return { .x = cell.x + 1, .y = cell.y };
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

    [[nodiscard]] constexpr bool operator==(this Wall, Wall) noexcept = default;
};

/** Return the wall in `direction` from `cell`. */
[[nodiscard]] constexpr Wall wall(Cell cell, Direction direction) {
    using enum Direction;
    using enum Orientation;
    switch (direction) {
    case up:
        return { .line = cell.y, .offset = cell.x, .orientation = horizontal };
    case down:
        return { .line = cell.y + 1, .offset = cell.x, .orientation = horizontal };
    case left:
        return { .line = cell.x, .offset = cell.y, .orientation = vertical };
    case right:
        return { .line = cell.x + 1, .offset = cell.y, .orientation = vertical };
    }
    throw std::runtime_error { "invalid direction" };
}

/** Return the two cells separated by `wall`.
 *
 * The cell which is left/up from the wall is the first one in the pair. */
[[nodiscard]] constexpr std::pair<Cell, Cell> neighbours(Wall wall) noexcept {
    switch (wall.orientation) {
    case Orientation::horizontal:
        return { { .x = wall.offset, .y = wall.line - 1 }, { .x = wall.offset, .y = wall.line } };
    case Orientation::vertical:
        return { { .x = wall.line - 1, .y = wall.offset }, { .x = wall.line, .y = wall.offset } };
    }
}
} // namespace GridMazes

template <>
struct std::hash<GridMazes::Cell> {
    std::size_t operator()(GridMazes::Cell cell) const noexcept {
        std::size_t seed { 0 };
        boost::hash_combine(seed, cell.x);
        boost::hash_combine(seed, cell.y);
        return seed;
    }
};

template <>
struct std::hash<GridMazes::Wall> {
    std::size_t operator()(GridMazes::Wall wall) const noexcept {
        std::size_t seed { 0 };
        boost::hash_combine(seed, wall.line);
        boost::hash_combine(seed, wall.offset);
        boost::hash_combine(seed, wall.orientation);
        return seed;
    }
};

template <>
struct std::formatter<GridMazes::Orientation> : std::formatter<std::string> {
    auto format(GridMazes::Orientation orientation, format_context& ctx) const {
        using enum GridMazes::Orientation;
        switch (orientation) {
        case horizontal:
            return formatter<string>::format(std::format("horizontal"), ctx);
        case vertical:
            return formatter<string>::format(std::format("vertical"), ctx);
        }
    }
};

template <>
struct std::formatter<GridMazes::Direction> : std::formatter<std::string> {
    auto format(GridMazes::Direction direction, format_context& ctx) const {
        using enum GridMazes::Direction;
        switch (direction) {
        case up:
            return formatter<string>::format(std::format("up"), ctx);
        case down:
            return formatter<string>::format(std::format("down"), ctx);
        case left:
            return formatter<string>::format(std::format("left"), ctx);
        case right:
            return formatter<string>::format(std::format("right"), ctx);
        default:
            return formatter<string>::format(std::format("unknown[{}]", std::to_underlying(direction)), ctx);
        }
    }
};

template <>
struct std::formatter<GridMazes::Wall> : std::formatter<std::string> {
    auto format(GridMazes::Wall wall, format_context& ctx) const {
        return formatter<string>::format(std::format("[{}, {}, {}]", wall.line, wall.offset, wall.orientation), ctx);
    }
};

template <>
struct std::formatter<GridMazes::Cell> : std::formatter<std::string> {
    auto format(GridMazes::Cell cell, format_context& ctx) const {
        return formatter<string>::format(std::format("[{}, {}]", cell.x, cell.y), ctx);
    }
};

inline std::ostream& operator<<(std::ostream& os, GridMazes::Direction direction) {
    return os << std::format("{}", direction);
}
inline std::ostream& operator<<(std::ostream& os, GridMazes::Orientation orientation) {
    return os << std::format("{}", orientation);
}
inline std::ostream& operator<<(std::ostream& os, GridMazes::Wall wall) { return os << std::format("{}", wall); }
inline std::ostream& operator<<(std::ostream& os, GridMazes::Cell cell) { return os << std::format("{}", cell); }
