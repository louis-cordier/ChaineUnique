#pragma once

#include "../order.hpp"
#include <vector>
#include <string>

/**
 * @brief Detailed diagnostic report produced by the Feasibility Judge.
 */
struct FeasibilityReport {
    bool is_feasible{false};
    uint64_t total_duration{0};
    size_t late_order_count{0};
    std::string first_late_job_id;
    uint64_t first_late_completion{0};
    uint64_t first_late_deadline{0};
};

/**
 * @brief Core Feasibility Judge (Arbitre).
 * 
 * Based on Jackson's Earliest Due Date (EDD) theorem:
 * A set of jobs is completely deliverable on time if and only if
 * sorting them by deadline d ascending yields zero delays.
 * 
 * @param orders The subset of orders to check.
 * @return true if 100% of orders complete on or before their deadlines, false otherwise.
 */
bool is_feasible(const std::vector<Order>& orders);

/**
 * @brief Evaluates feasibility and produces a diagnostic report.
 */
FeasibilityReport evaluate_feasibility(const std::vector<Order>& orders);
