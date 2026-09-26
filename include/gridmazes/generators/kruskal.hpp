/** Kruskal's algorithm-based maze generator.
 *
 * Source:
 * https://en.wikipedia.org/wiki/Kruskal's_algorithm */
#pragma once
#include "gridmazes/generators/common.hpp"
#include "gridmazes/maze.hpp"

#include <algorithm>
#include <ranges>
#include <unordered_set>
#include <vector>

namespace GridMazes {

namespace Private {
using CellGroup = std::unordered_set<Cell>;
using CellGroupMap = std::unordered_map<Cell, std::shared_ptr<CellGroup>>;

[[nodiscard]] CellGroupMap make_cell_groups(const Maze& maze);
} // namespace Private

/** Represents a Kruskal's algorithm maze generator.
 *
 * `RNG` should satisfy the RandomNumberEngine named requirement. */
class KruskalGenerator : public CommonGenerator {
  public:
    template <typename RNG>
    KruskalGenerator(int width, int height, RNG&& rng)
        : CommonGenerator(width, height), m_walls { std::from_range, internal_walls(m_maze) },
          m_groups_by_cell { Private::make_cell_groups(m_maze) } {
        // Start with all walls set.
        m_maze.set_all();
        // Shuffle the walls now so we don't have to keep a pointer to rng.
        std::ranges::shuffle(m_walls, std::forward<RNG>(rng));
    }

  protected:
    void step_impl() override;

  private:
    std::vector<Wall> m_walls;
    Private::CellGroupMap m_groups_by_cell;
    std::size_t m_position { 0 };
};

} // namespace GridMazes
