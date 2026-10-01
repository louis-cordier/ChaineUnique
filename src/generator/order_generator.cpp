#include "order_generator.hpp"
#include "prng.hpp"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>

GenerationResult generate_order_book(const GeneratorConfig& config) {
    Xorshift32 prng(config.seed);
    std::vector<Order> all_orders;
    all_orders.reserve(config.num_planted + config.num_noise);

    // 1. Generate planted orders (guaranteed 100% on time by construction)
    uint64_t cumulative_time = 0;
    for (size_t i = 1; i <= config.num_planted; ++i) {
        uint64_t p = prng.next_range(config.p_min, config.p_max);
        cumulative_time += p;
        uint64_t slack = prng.next_range(0, config.slack_max);
        uint64_t d = cumulative_time + slack;
        uint64_t w = prng.next_range(config.w_min, config.w_max);
        std::string id = config.prefix + "P" + std::to_string(i);

        all_orders.push_back(Order{id, p, d, w, 0});
    }

    // 2. Generate noise orders (parasitic tasks with tight deadlines to saturate the machine)
    uint64_t max_noise_deadline = std::max<uint64_t>(1, cumulative_time / 2);
    for (size_t j = 1; j <= config.num_noise; ++j) {
        uint64_t p = prng.next_range(config.p_min, config.p_max);
        uint64_t d = prng.next_range(1, max_noise_deadline);
        uint64_t w = prng.next_range(config.w_min, config.w_max);
        std::string id = config.prefix + "N" + std::to_string(j);

        all_orders.push_back(Order{id, p, d, w, 0});
    }

    // 3. Deterministic Fisher-Yates shuffle using PRNG
    for (size_t i = all_orders.size(); i > 1; --i) {
        size_t j = prng.next_range(0, i - 1);
        std::swap(all_orders[i - 1], all_orders[j]);
    }

    // 4. Assign 1-based original line numbers
    for (size_t i = 0; i < all_orders.size(); ++i) {
        all_orders[i].original_line = i + 1;
    }

    GenerationResult result;
    result.orders = std::move(all_orders);
    result.planted_count = config.num_planted;
    result.noise_count = config.num_noise;
    result.seed = config.seed;

    return result;
}

void save_order_book(const std::string& filepath, const std::vector<Order>& orders) {
    std::ofstream out(filepath);
    if (!out.is_open()) {
        throw std::runtime_error("Could not write order book to: " + filepath);
    }

    for (const auto& o : orders) {
        out << o.id << " " << o.processing_time << " " << o.deadline << " " << o.penalty << "\n";
    }
}

void save_witness_file(const std::string& filepath, size_t planted_count, uint32_t seed) {
    std::ofstream out(filepath);
    if (!out.is_open()) {
        throw std::runtime_error("Could not write witness file to: " + filepath);
    }

    out << "# WITNESS BENCHMARK FILE (External verification only)\n"
        << "# DO NOT READ OR USE BY SCHEDULING ALGORITHMS\n"
        << "seed " << seed << "\n"
        << "planted_count " << planted_count << "\n";
}
