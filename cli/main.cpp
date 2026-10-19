#include "io.hpp"
#include "random.hpp"

#include <CLI/CLI.hpp>
#include <gridmazes/generators/generator.hpp>
#include <gridmazes/generators/kruskal.hpp>
#include <gridmazes/maze.hpp>
#include <gridmazes/renderers/text.hpp>

int main(int argc, char** argv) {
    using namespace GridMazes;

    CLI::App app { "Playground for 2D grid mazes", "gridmazes" };
    app.require_subcommand(1);

    auto* generate_cmd { app.add_subcommand("generate", "Generate a maze.") };

    int width {};
    generate_cmd->add_option("width", width, "Width of the generated maze")->required();

    int height {};
    generate_cmd->add_option("height", height, "Height of the generated maze")->required();

    bool showSteps { false };
    generate_cmd->add_flag("--steps", showSteps, "If specified, show each step of the algorithm");

    generate_cmd->callback([&]() {
        if (!showSteps) {
            ::print(generate<KruskalGenerator>(width, height, ::create_rng()));
            return;
        }

        KruskalGenerator generator { width, height, ::create_rng() };
        ::print(generator.state());
        while (!generator.has_terminated()) {
            ::wait_for_enter();
            ::print(generator.step());
        }
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
