#include "gridmazes/generators/common.hpp"

#include "gridmazes/maze.hpp"

namespace GridMazes {

CommonGenerator::CommonGenerator(int width, int height)
    : m_maze { width, height }, m_terminated { width == 1 || height == 1 } {}

const Maze& CommonGenerator::step() {
    if (!has_terminated()) {
        step_impl();
    }
    return m_maze;
}

const Maze& CommonGenerator::run() {
    while (!has_terminated()) {
        step_impl();
    }
    return m_maze;
}
} // namespace GridMazes
