#pragma once

#include <string>
#include <cstdint>
#include <iostream>

/**
 * @brief Represents a single customer job/order in the single-machine workshop.
 * 
 * Each order has:
 * - id: unique identifier string (without spaces)
 * - processing_time: duration p needed on the machine (strictly positive integer)
 * - deadline: contractual due date d (strictly positive integer)
 * - penalty: financial penalty w if completed after deadline (strictly positive integer)
 * - original_line: 1-based line number in the source file, mandatory for tie-breaking
 */
struct Order {
    std::string id;
    uint64_t processing_time{0}; // p
    uint64_t deadline{0};        // d
    uint64_t penalty{0};         // w
    size_t original_line{0};     // Line number in original input file

    /**
     * @brief Penalty-to-duration ratio (w / p) used by heuristic R3'.
     */
    double ratio() const {
        if (processing_time == 0) return 0.0;
        return static_cast<double>(penalty) / static_cast<double>(processing_time);
    }
};

/**
 * @brief Pretty print an Order to an output stream.
 */
inline std::ostream& operator<<(std::ostream& os, const Order& o) {
    os << "[" << o.id << ": p=" << o.processing_time 
       << ", d=" << o.deadline 
       << ", w=" << o.penalty 
       << ", line=" << o.original_line << "]";
    return os;
}
