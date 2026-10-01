#pragma once

#include "../order.hpp"
#include <vector>
#include <string>
#include <cstdint>

/**
 * @brief Configuration parameters for reproducible order book generation.
 */
struct GeneratorConfig {
    uint32_t seed{42};
    size_t num_planted{10};       // Number of jobs guaranteed on-time by construction
    size_t num_noise{5};          // Number of noise/parasite jobs with tight deadlines
    uint64_t p_min{1};            // Minimum processing time
    uint64_t p_max{10};           // Maximum processing time
    uint64_t w_min{5};            // Minimum penalty
    uint64_t w_max{50};           // Maximum penalty
    uint64_t slack_max{3};        // Max extra deadline buffer beyond cumulative completion time
    std::string prefix{"JOB_"};   // Order ID prefix
};

/**
 * @brief Result of order book generation.
 */
struct GenerationResult {
    std::vector<Order> orders;
    size_t planted_count{0};
    size_t noise_count{0};
    uint32_t seed{42};
};

/**
 * @brief Generates an order book with planted feasible solution and noise orders.
 * 
 * 1. Creates planted jobs with deadlines d_i >= cumulative sum of p.
 * 2. Injects noise jobs with tight deadlines to saturate the machine.
 * 3. Shuffles all jobs deterministically with the PRNG.
 */
GenerationResult generate_order_book(const GeneratorConfig& config);

/**
 * @brief Writes orders to a file in format: ID p d w
 */
void save_order_book(const std::string& filepath, const std::vector<Order>& orders);

/**
 * @brief Writes the witness file containing only the external planted benchmark count.
 */
void save_witness_file(const std::string& filepath, size_t planted_count, uint32_t seed);
