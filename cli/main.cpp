#include "io.hpp"
#include "random.hpp"

#include <CLI/CLI.hpp>
#include <gridmazes/generators/common.hpp>
#include <gridmazes/generators/kruskal.hpp>
#include <gridmazes/generators/prim.hpp>
#include <gridmazes/generators/recursive_division.hpp>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {
[[nodiscard]] std::unique_ptr<GridMazes::CommonGenerator> get_generator(std::string_view name, int width, int height) {
    using namespace GridMazes;
    if (name == "kruskal") {
        return std::make_unique<KruskalGenerator>(width, height, ::create_rng());
    } else if (name == "recursive-division") {
        return std::make_unique<RecursiveDivisionGenerator<std::mt19937>>(width, height, ::create_rng());
    } else if (name == "prim") {
        return std::make_unique<PrimGenerator<std::mt19937>>(width, height, ::create_rng());
    }
    throw std::runtime_error { "Invalid generator type." };
}
} // namespace

int main(int argc, char* const* argv) {
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

    std::string generator_name { "kruskal" };
    generate_cmd->add_option("--generator", generator_name, "The type of generator to use")
        ->check(CLI::IsMember({ "kruskal", "recursive-division", "prim" }));

    generate_cmd->callback([&] {
        std::unique_ptr<CommonGenerator> generator { ::get_generator(generator_name, width, height) };

        if (!showSteps) {
            ::print(generator->run());
            return;
        }

        ::print(generator->state());
        while (!generator->has_terminated()) {
            ::wait_for_enter();
            ::print(generator->step());
        }
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
