#include "gridmazes/generators/generator.hpp"
#include "gridmazes/generators/kruskal.hpp"
#include "gridmazes/maze.hpp"
#include "gridmazes/renderers/text.hpp"
#include "random.hpp"

#include <CLI/CLI.hpp>
#include <print>

int main(int argc, char** argv) {
    using namespace GridMazes;

    CLI::App app { "Playground for 2D grid mazes", "gridmazes" };
    app.require_subcommand(1);

    auto* generate_cmd { app.add_subcommand("generate", "Generate a maze.") };

    int width {};
    generate_cmd->add_option("width", width, "Width of the generated maze")->required();

    int height {};
    generate_cmd->add_option("height", height, "Height of the generated maze")->required();

    generate_cmd->callback([&]() {
        const Maze maze { generate<KruskalGenerator>(width, height, create_rng()) };
        std::println("{}", to_text(maze));
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
