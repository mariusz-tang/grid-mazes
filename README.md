# Grid Mazes

C++ library for generating and solving 2D grid mazes.

## Goals

This project is a work-in-progess. This is what is implemented so far:

- Basic maze data structure.
- Maze-to-text renderer.
- Kruskal's algorithm maze generator.
- Maze connectedness checker.
- Basic command-line interface.

Here are some planned features:

- more maze generation algorithms.
- maze solving algorithms.
- Maze-to-image renderer.
- GUI to showcase the algorithms plus interactive features like manual maze input.
- Benchmarking.

## Dependencies

- Boost (for dynamic bitsets and hashing utilities).

### Downloaded using `FetchContent`

- Catch2 (for unit tests).
- CLI11 (for the command-line parser).

## Setup

This project uses CMake:

```bash
cmake --preset default
cmake --build build
```

In case an error prevents the build, you can try disabling treating warnings as
errors:

```bash
cmake --compile-no-warning-as-error build
cmake --build build
```

### Command-line interface

This project comes with a command-line interface! Try it out:

```bash
# See the help.
gridmazes-cli --help
# See the help for a subcommand.
gridmazes-cli --help generate
# Generate a 5x5 maze.
gridmazes-cli generate 5 5
```

Using the default preset, the executable is placed in `./build/cli/gridmazes-cli`.

### Unit tests

Run CTest after building the project:

```bash
ctest --test-dir build
```

### Prek hooks

We use prek to run pre-commit hooks:

```bash
prek --install
# Optionally run the hooks on the whole project immediately.
prek --all-files
```

There is no `clang-tidy` hook enabled because I have consistently experienced
issues using them, including false positivies and inconsistent output compared
to `run-clang-tidy`. Please run `clang-tidy` manually as needed.
