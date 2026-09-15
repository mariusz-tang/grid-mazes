#pragma once
#include "gridmazes/grid.hpp"

#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <cstddef>
#include <format>
#include <generator>
#include <utility>

namespace GridMazes {

/** Represents a 2D rectangular grid maze.
 *
 * A maze has positive integer width and height.
 *
 * The maze "contains" all of the cells in the rectangle with corners [0,0]
 * and [width-1, height-1], inclusive, as well as all of those cells' walls.
 * These cells are said to be "internal" while all others are "external".
 *
 * Walls which separate two internal cells are also said to be "internal". Walls
 * which separate an internal cell from an external cell are said to be "boundary".
 * All other walls are said to be "external". Therefore, the maze contains exactly
 * those walls which are not external.
 *
 * Walls can be "set" (impassable) or "unset" (passable). External walls are
 * always unset, boundary walls are always set, and internal walls may be set
 * or unset. */
class Maze {
  public:
    /** Initialize a `Maze` instance with `height` rows, `width` columns.
     *
     * `height` and `width` must both be greater than 0.
     *
     * The resulting instance has all internal walls passable. */
    Maze(int width, int height);

    [[nodiscard]] constexpr bool operator==(const Maze&) const noexcept = default;

    /** Get the number of columns. */
    [[nodiscard]] int width() const noexcept { return m_width; }
    /** Get the number of rows. */
    [[nodiscard]] int height() const noexcept { return m_height; }
    /** Get the number of cells in the maze */
    [[nodiscard]] std::size_t size() const noexcept { return static_cast<std::size_t>(m_width) * m_height; }

    /** Return true if `cell` is within the bounds of this maze. */
    [[nodiscard]] bool contains(Cell cell) const noexcept;
    /** Return true if `wall` is a boundary or internal wall. */
    [[nodiscard]] bool contains(Wall wall) const noexcept;

    /** Return true if the wall is on the boundary of the maze. */
    [[nodiscard]] bool is_boundary(Wall wall) const noexcept;
    /** Return true if the wall is within, but not on, the boundary of the maze. */
    [[nodiscard]] bool is_internal(Wall wall) const noexcept;

    /** Return all of the cells contained within the bounds of the maze.
     *
     * Each cell is returned exactly once, in an unspecified order. */
    [[nodiscard]] std::generator<Cell> cells() const noexcept;
    /** Return direction/neighbour pairs for each reachable cell adjancent to `cell`.
     *
     * A neighbour is "reachable" if the wall between it and `cell` is passable. */
    [[nodiscard]] std::generator<std::pair<Cell, Direction>> neighbours(Cell cell) const;

    /** Return all of the boundary and internal walls of the maze.
     *
     * Each wall is returned exactly once, in an unspecified order. */
    [[nodiscard]] std::generator<Wall> walls() const noexcept;
    /** Return all of the internal walls of the maze.
     *
     * Each wall is returned exactly once, in an unspecified order. */
    [[nodiscard]] std::generator<Wall> internal_walls() const noexcept;
    /** Return all of the boundary walls of the maze.
     *
     * Each wall is returned exactly once, in an unspecified order. */
    [[nodiscard]] std::generator<Wall> boundary_walls() const noexcept;

    /** Return true if the wall is set.
     *
     * External walls are always unset, and boundary walls are always set. */
    [[nodiscard]] bool is_set(Wall wall) const noexcept;

    /** Make `wall` impassable.
     *
     * `wall` must be an internal wall. */
    void set(Wall wall);
    /** Make `wall` passable.
     *
     * `wall` must be an internal wall. */
    void unset(Wall wall);
    /** Make `wall` passable if it was impassable, and vice versa.
     *
     * `wall` must be an internal wall. */
    void toggle(Wall wall);

    /** Make all internal walls impassable. */
    void set_all() noexcept { m_internalWalls.set(); }
    /** Return true if all internal walls are impassable. */
    [[nodiscard]] bool all_set() const noexcept { return m_internalWalls.all(); }

    /** Make all internal walls passable. */
    void unset_all() noexcept { m_internalWalls.reset(); }
    /** Return true if all internal walls are passable. */
    [[nodiscard]] bool all_unset() const noexcept { return m_internalWalls.none(); }

  private:
    int m_width;
    int m_height;
    boost::dynamic_bitset<> m_internalWalls;
};
} // namespace GridMazes

template <>
struct std::formatter<GridMazes::Maze> : std::formatter<std::string> {
    auto format(const GridMazes::Maze& maze, format_context& ctx) const {
        return formatter<string>::format(std::format("maze of width {} and height {}", maze.width(), maze.height()),
                                         ctx);
    }
};

inline std::ostream& operator<<(std::ostream& os, const GridMazes::Maze& maze) { return os << std::format("{}", maze); }
