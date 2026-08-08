#pragma once
#include "gridmazes/position.hpp"

#include <boost/dynamic_bitset/dynamic_bitset.hpp>

namespace GridMazes {
/** Represents a 2D grid maze. */
class Maze {
  public:
    /** Initialize a `Maze` instance with `height` rows, `width` columns.
     *
     * External walls are always present.
     *
     * `height` and `width` must both be greater than 0.
     *
     * The resulting instance has no internal walls. */
    Maze(int width, int height);

    /** Get the number of columns. */
    [[nodiscard]] int width() const noexcept { return m_width; }

    /** Get the number of rows. */
    [[nodiscard]] int height() const noexcept { return m_height; }

    /** Return true if `position` is within the bounds of this maze. */
    [[nodiscard]] bool contains_position(Position position) const noexcept;

    /** Return true if the wall is on the boundary of the maze.
     *
     * The maze must contain `position`.
     * */
    [[nodiscard]] bool is_external_wall(Position position, Direction direction) const;

    /** Return true if the wall is within the boundary of the maze.
     *
     * The maze must contain `position`.
     * */
    [[nodiscard]] bool is_internal_wall(Position position, Direction direction) const {
        return !is_external_wall(position, direction);
    }

    /** Return true if there is a wall in `direction` from `position`.
     *
     * The maze must contain `position`.
     * */
    [[nodiscard]] bool has_wall(Position position, Direction direction) const;

    /** Set the wall in `direction` from `position` to true.
     *
     * The maze must contain `position` and the wall must be internal.
     * */
    void place_wall(Position position, Direction direction) { set_wall(position, direction, true); }

    /** Set the wall in `direction` from `position` to false.
     *
     * The maze must contain `position` and the wall must be internal.
     * */
    void remove_wall(Position position, Direction direction) { set_wall(position, direction, false); }

    /** Set the wall in `direction` from `position`.
     *
     * The maze must contain `position` and the wall must be internal.
     * */
    void set_wall(Position position, Direction direction, bool state);

  private:
    int m_width;
    int m_height;
    boost::dynamic_bitset<> m_internal_walls;

    [[nodiscard]] int get_wall_index(Position position, Direction direction) const;
};
} // namespace GridMazes
