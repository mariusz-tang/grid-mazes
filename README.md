# Grid Mazes

C++ library for generating and solving 2D grid mazes.

## Dependencies

- Boost (for dynamic bitsets and hashing utilities).
- Catch2 (for unit tests). (downloaded using `FetchContent`)

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
