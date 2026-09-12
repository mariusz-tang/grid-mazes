#pragma once
#include "gridmazes/grid.hpp"

#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <format>
#include <generator>

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

    /** Get the number of columns. */
    [[nodiscard]] int width() const noexcept { return m_width; }

    /** Get the number of rows. */
    [[nodiscard]] int height() const noexcept { return m_height; }

    /** Return true if `cell` is within the bounds of this maze. */
    [[nodiscard]] bool contains(const Cell& cell) const noexcept;
    /** Return true if `wall` is a boundary or internal wall. */
    [[nodiscard]] bool contains(const Wall& wall) const noexcept;

    /** Return true if the wall is on the boundary of the maze. */
    [[nodiscard]] bool is_boundary(const Wall& wall) const noexcept;
    /** Return true if the wall is within, but not on, the boundary of the maze. */
    [[nodiscard]] bool is_internal(const Wall& wall) const noexcept;

    /** Return all of the cells contained within the bounds of the maze.
     *
     * Each cell is returned exactly once, in an unspecified order. */
    [[nodiscard]] std::generator<const Cell&> cells() const noexcept;
    /** Return all of the boundary and internal walls of the maze.
     *
     * Each wall is returned exactly once, in an unspecified order. */
    [[nodiscard]] std::generator<const Wall&> walls() const noexcept;
    /** Return all of the internal walls of the maze.
     *
     * Each wall is returned exactly once, in an unspecified order. */
    [[nodiscard]] std::generator<const Wall&> internal_walls() const noexcept;
    /** Return all of the boundary walls of the maze.
     *
     * Each wall is returned exactly once, in an unspecified order. */
    [[nodiscard]] std::generator<const Wall&> boundary_walls() const noexcept;

    /** Return true if the wall is set.
     *
     * External walls are always unset, and boundary walls are always set. */
    [[nodiscard]] bool is_set(const Wall& wall) const noexcept;

    /** Make `wall` impassable.
     *
     * `wall` must be an internal wall. */
    void set(const Wall& wall);
    /** Make `wall` passable.
     *
     * `wall` must be an internal wall. */
    void unset(const Wall& wall);
    /** Make `wall` passable if it was impassable, and vice versa.
     *
     * `wall` must be an internal wall. */
    void toggle(const Wall& wall);

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
