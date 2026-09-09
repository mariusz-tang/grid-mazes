/** Kruskal's algorithm-based maze generator.
 *
 * Source:
 * https://en.wikipedia.org/wiki/Kruskal's_algorithm */
#pragma once
#include "gridmazes/maze.hpp"

#include <generator>
#include <vector>

namespace GridMazes::Generators {
namespace Private {
[[nodiscard]] std::generator<const Maze&> kruskal_steps(Maze maze, std::vector<Wall> orderedWalls);
[[nodiscard]] Maze kruskal(Maze& maze, const std::vector<Wall>& orderedWalls);
} // namespace Private

/** Return each step of generating a `width` by `height` maze using Kruskal's algorithm.
 *
 * `RNG` should satisfy RandomNumberEngine. */
template <typename RNG>
[[nodiscard]] std::generator<const Maze&> kruskal_steps(int width, int height, RNG&& rng) {
    Maze maze { width, height };
    std::vector walls(std::from_range, maze.walls());
    std::ranges::shuffle(walls, std::forward<RNG>(rng));
    return Private::kruskal_steps(std::move(maze), std::move(walls));
}

/** Generate a `width` by `height` maze using Kruskal's algorithm.
 *
 * `RNG` should satisfy RandomNumberEngine. */
template <typename RNG>
[[nodiscard]] Maze kruskal(int width, int height, RNG&& rng) {
    Maze maze { width, height };
    std::vector walls(std::from_range, maze.walls());
    std::ranges::shuffle(walls, std::forward<RNG>(rng));
    return Private::kruskal(maze, walls);
}
} // namespace GridMazes::Generators
