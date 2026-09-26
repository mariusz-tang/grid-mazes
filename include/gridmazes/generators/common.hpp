/** Abstract base classes for maze generators. */
#pragma once
#include "gridmazes/maze.hpp"

#include <memory>

namespace GridMazes {

/** Abstract class representing a maze generation algorithm taking maze shape as input.
 *
 * Satisfies `Generator`*/
class CommonGenerator {
  public:
    /** Initialize this algorithm with a `width` by `height` maze.
     *
     * Marks the algorithm as terminated if either of `width` or `height` is equal to 1,
     * since the maze is already a fully connected tree at this point.
     * Child classes can overwrite `m_terminated` to override this behaviour. */
    CommonGenerator(int width, int height);

    CommonGenerator(const CommonGenerator&) = default;
    CommonGenerator(CommonGenerator&&) noexcept = default;
    CommonGenerator& operator=(const CommonGenerator&) = default;
    CommonGenerator& operator=(CommonGenerator&&) noexcept = default;
    virtual ~CommonGenerator() = default;

    /** The current state of the maze. */
    [[nodiscard]] const Maze& state() const noexcept { return m_maze; }

    /** Execute one step of the algorithm and return the new state of the maze. */
    const Maze& step();

    /** Execute the algorithm until termination and return the new state of the maze. */
    const Maze& run();

    /** Return true if the algorithm has finished.
     *
     * If true, subsequent calls to `step` and `run` will have no effect on the state of the maze. */
    [[nodiscard]] bool has_terminated() const { return m_terminated; }

  protected:
    Maze m_maze;
    bool m_terminated;

    /** Execute one step of the algorithm and set `m_terminated` if the algorithm terminates after this step.
     *
     * Child classes should define this function to specify the algorithm behaviour. */
    virtual void step_impl() = 0;
};
} // namespace GridMazes
