#include <gridmazes/maze.hpp>
#include <gridmazes/renderers/text.hpp>
#include <iostream>
#include <print>

inline void print(const GridMazes::Maze& maze) { std::println("{}", to_text(maze)); }

inline void wait_for_enter() {
    std::println("Press ENTER to continue");
    std::cin.get();
}
