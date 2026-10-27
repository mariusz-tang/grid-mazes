/** Recursive division maze generator. */
#pragma once
#include "gridmazes/generators/common.hpp"
#include "gridmazes/grid.hpp"

#include <random>
#include <stack>

namespace GridMazes {

namespace Private {
// Rectangular cell group, represented as the position of its upperleftmost cell,
// and its width and height.
struct Rectangle {
    int x;
    int y;
    int width;
    int height;
};
} // namespace Private

/** Represents a recursive division algorithm maze generator.
 *
 * `RNG` should satisfy the RandomNumberEngine named requirement.
 *
 * The algorithm works by recursively partitioning the maze into subgroups, starting
 * with the entire maze as one big group. When groups are partitioned, one wall is
 * left passable so that the maze remains connected. Groups are partitioned until
 * they have width or height equal to 1. */
template <typename RNG>
class RecursiveDivisionGenerator : public CommonGenerator {
  public:
    RecursiveDivisionGenerator(int width, int height, RNG&& rng)
        : CommonGenerator(width, height), m_rng { std::move(rng) },
          m_groups { { Private::Rectangle { .x = 0, .y = 0, .width = width, .height = height } } } {}

  protected:
    void step_impl() override {
        // Choose the next group to split.
        const Private::Rectangle group { m_groups.top() };
        m_groups.pop();

        // Pick the dimension which is larger, or pick one at random if equal.
        // If height is larger, split vertically.
        auto orientation { Orientation::horizontal };
        int lineStart { group.y };
        int lineSize { group.height - 1 };
        int offsetStart { group.x };
        int offsetSize { group.width };
        // If width is larger, split horizontally.
        if (group.width > group.height ||
            (group.width == group.height && std::uniform_int_distribution<>(0, 1)(m_rng))) {
            orientation = Orientation::vertical;
            lineStart = group.x;
            lineSize = group.width - 1;
            offsetStart = group.y;
            offsetSize = group.height;
        }

        // Pick the size of one of the new groups after division, and the position
        // of the break (to allow passage between the new subgroups).
        const int newGroupSize { std::uniform_int_distribution<>(1, lineSize)(m_rng) };
        const int line { lineStart + newGroupSize };
        const int breakOffset { offsetStart + std::uniform_int_distribution<>(0, offsetSize - 1)(m_rng) };

        // Split the group in two, leaving one wall passable.
        for (int offset { offsetStart }; offset < offsetStart + offsetSize; offset++) {
            if (offset != breakOffset) {
                m_maze.set(Wall { .line = line, .offset = offset, .orientation = orientation });
            }
        }

        // Add the new subgroups to the stack, but only if they have both width
        // and height greater than 1.
        switch (orientation) {
        case Orientation::horizontal:
            if (group.width != 1 && newGroupSize != 1) {
                m_groups.push({ .x = group.x, .y = group.y, .width = group.width, .height = newGroupSize });
            }
            if (group.width != 1 && group.height - newGroupSize != 1) {
                m_groups.push({ .x = group.x, .y = line, .width = group.width, .height = group.height - newGroupSize });
            }
            break;
        case Orientation::vertical:
            if (group.height != 1 && newGroupSize != 1) {
                m_groups.push({ .x = group.x, .y = group.y, .width = newGroupSize, .height = group.height });
            }
            if (group.height != 1 && group.width - newGroupSize != 1) {
                m_groups.push({ .x = line, .y = group.y, .width = group.width - newGroupSize, .height = group.height });
            }
        }

        // If there are no groups left then we are finished.
        if (m_groups.empty()) {
            m_terminated = true;
        }
    }

  private:
    RNG m_rng;
    std::stack<Private::Rectangle> m_groups;
};

} // namespace GridMazes
