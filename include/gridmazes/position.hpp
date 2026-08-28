#pragma once
#include <array>
#include <cstdint>
#include <format>
#include <string>

namespace GridMazes { /** Represents a direction in a maze. */
enum class Direction : std::uint8_t { up, down, left, right };

inline constexpr std::array directions { Direction::up, Direction::down, Direction::left, Direction::right };

/** Return the opposite direction. */
[[nodiscard]] Direction get_opposite(Direction direction);

/** Represents a cell position in a maze. */
struct Position {
    int column;
    int row;

    /** Return the next `Position` in `direction`. */
    [[nodiscard]] Position get_neighbour(Direction direction) const;

    [[nodiscard]] bool operator==(const Position& other) const noexcept = default;
};

} // namespace GridMazes

template <>
struct std::formatter<GridMazes::Position> : std::formatter<std::string> {
    auto format(GridMazes::Position position, format_context& ctx) const {
        return formatter<string>::format(std::format("({}, {})", position.column, position.row), ctx);
    }
};
