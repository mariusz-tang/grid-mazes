/** Prim's algorithm-based maze generator.
 *
 * Source:
 * https://en.wikipedia.org/wiki/Prim's_algorithm */
#pragma once
#include "gridmazes/generators/common.hpp"
#include "gridmazes/grid.hpp"
#include "gridmazes/maze.hpp"

#include <cassert>
#include <unordered_set>
#include <utility>
#include <vector>

namespace GridMazes {

/** Represents a recursive division algorithm maze generator.
 *
 * `RNG` should satisfy the RandomNumberEngine named requirement.
 *
 * The algorithm works by recursively partitioning the maze into subgroups, starting
 * with the entire maze as one big group. When groups are partitioned, one wall is
 * left passable so that the maze remains connected. Groups are partitioned until
 * they have width or height equal to 1. */
template <typename RNG>
class PrimGenerator : public CommonGenerator {
  public:
    PrimGenerator(int width, int height, RNG&& rng) : CommonGenerator(width, height), m_rng { std::move(rng) } {
        // Start with all the walls set.
        m_maze.set_all();

        // Pick a random starting cell.
        const Cell start {
            std::uniform_int_distribution<>(0, width - 1)(m_rng),
            std::uniform_int_distribution<>(0, height - 1)(m_rng),
        };

        // Mark the cell as visited and add its neighbours to the adjacency list.
        m_visitedCells.insert(start);
        for (const auto& direction : directions) {
            const Cell nbour { neighbour(start, direction) };
            if (contains(m_maze, nbour)) {
                m_adjacentCells.push_back(nbour);
                m_adjacentSet.insert(nbour);
            }
        }
    }

  protected:
    void step_impl() override {
        assert(!m_adjacentCells.empty());
        // Choose an adjacent cell and mark it as visited.
        const int index { std::uniform_int_distribution<>(0, m_adjacentCells.size() - 1)(m_rng) };
        const Cell cell { m_adjacentCells[index] };
        m_visitedCells.insert(cell);

        // Remove the cell from the adjacency list using the remove-erase idiom.
        // std::erase is O(n) but this is O(1); this only possible because we
        // know the index of the element to erase.
        std::swap(m_adjacentCells[index], m_adjacentCells[m_adjacentCells.size() - 1]);
        m_adjacentCells.resize(m_adjacentCells.size() - 1);
        m_adjacentSet.erase(cell);

        // Find possible directions in which we can connect to the visited cell set.
        std::vector<Direction> validDirections {};
        for (const auto& direction : directions) {
            const Cell nbour { neighbour(cell, direction) };
            if (m_visitedCells.contains(nbour)) {
                validDirections.push_back(direction);
            }
        }
        assert(!validDirections.empty());
        const Direction direction {
            validDirections[std::uniform_int_distribution<>(0, static_cast<int>(validDirections.size()) - 1)(m_rng)],
        };
        // Connect the new cell to the visited cell set.
        m_maze.unset(wall(cell, direction));

        // Add any new adjacent cells.
        for (const auto& direction : directions) {
            const Cell nbour { neighbour(cell, direction) };
            if (contains(m_maze, nbour) && !m_adjacentSet.contains(nbour) && !m_visitedCells.contains(nbour)) {
                m_adjacentCells.push_back(nbour);
                m_adjacentSet.insert(nbour);
            }
        }

        // If there are no more adjacent cells then we are done.
        if (m_adjacentCells.empty()) {
            m_terminated = true;
        }
    }

  private:
    RNG m_rng;
    // Use both a vector and a set for random access and O(1) search on adjacent cells.
    std::vector<Cell> m_adjacentCells;
    std::unordered_set<Cell> m_adjacentSet;
    std::unordered_set<Cell> m_visitedCells;
};

} // namespace GridMazes
