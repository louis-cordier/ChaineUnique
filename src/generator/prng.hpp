#pragma once

#include <cstdint>

/**
 * @brief Fast, reproducible 32-bit Pseudo-Random Number Generator (PRNG).
 * 
 * Implements the Marsaglia Xorshift32 algorithm.
 * Deterministic across all platforms, driven by a 32-bit integer seed.
 */
class Xorshift32 {
private:
    uint32_t state;

public:
    /**
     * @brief Initialize generator with a seed.
     * If seed is 0, a non-zero default fallback is used (xorshift requires state != 0).
     */
    explicit Xorshift32(uint32_t seed = 42) : state(seed != 0 ? seed : 0xDEADBEEFu) {}

    /**
     * @brief Generate next pseudo-random 32-bit unsigned integer.
     */
    uint32_t next_uint32() {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }

    /**
     * @brief Generate a random integer in closed interval [min_val, max_val].
     */
    uint64_t next_range(uint64_t min_val, uint64_t max_val) {
        if (min_val >= max_val) {
            return min_val;
        }
        uint64_t range = max_val - min_val + 1;
        return min_val + (static_cast<uint64_t>(next_uint32()) % range);
    }
};
