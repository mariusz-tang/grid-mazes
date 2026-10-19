/** Maze generator concept and helper functions. */
#pragma once
#include "gridmazes/maze.hpp"

#include <concepts>
#include <generator>

namespace GridMazes {

/** Represents a maze generation algorithm.
 *
 * An algorithm can be stepped through, and the current maze state can
 * be queried at any time. An algorithm can also be run until termination. */
template <typename T>
concept Generator = requires(T generator) {
    /** The current state of the maze. */
    { generator.state() } -> std::convertible_to<const Maze&>;
    /** Execute one step of the algorithm and return the new maze state. */
    { generator.step() } -> std::convertible_to<const Maze&>;
    /** Execute the algorithm until termination and return the new maze state. */
    { generator.run() } -> std::convertible_to<const Maze&>;
    /** Return true if the algorithm has finished.
     *
     * If true, subsequent calls to `step` and `run` will have no effect on the maze state. */
    { generator.has_terminated() } -> std::same_as<bool>;
};

/** Generate a maze using an algorithm with constructor arguments `args`, returning the maze state at each step. */
template <Generator T, typename... Args>
[[nodiscard]] std::generator<const Maze&> steps(Args&&... args) {
    T generator { std::forward<Args>(args)... };
    while (!generator.has_terminated()) {
        co_yield generator.step();
    }
}

/** Generate a maze using an algorithm with constructor arguments `args`. */
template <Generator T, typename... Args>
[[nodiscard]] Maze generate(Args&&... args) {
    T generator { std::forward<Args>(args)... };
    return generator.run();
}

} // namespace GridMazes
