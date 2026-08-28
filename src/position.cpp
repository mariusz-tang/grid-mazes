#include "gridmazes/position.hpp"

#include <stdexcept>

namespace GridMazes {

using enum Direction;

Direction get_opposite(Direction direction) {
    switch (direction) {
    case up:
        return down;
    case down:
        return up;
    case left:
        return right;
    case right:
        return left;
    }
    throw std::runtime_error { "invalid direction" };
}

Position Position::get_neighbour(Direction direction) const {
    switch (direction) {
    case up:
        return { .column = column, .row = row - 1 };
    case down:
        return { .column = column, .row = row + 1 };
    case left:
        return { .column = column - 1, .row = row };
    case right:
        return { .column = column + 1, .row = row };
    }
    throw std::runtime_error { "invalid direction" };
}

} // namespace GridMazes
