/** Random number generator.
 *
 * Adapted from: https://www.learncpp.com/cpp-tutorial/global-random-numbers-random-h/ */
#pragma once
#include <chrono>
#include <random>

inline std::mt19937 create_rng() {
    std::random_device rnd {};

    std::seed_seq seed {
        static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
        rnd(),
        rnd(),
        rnd(),
        rnd(),
        rnd(),
        rnd(),
        rnd(),
    };

    return std::mt19937 { seed };
}
