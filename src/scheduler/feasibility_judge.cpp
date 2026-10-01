#include "feasibility_judge.hpp"
#include <algorithm>

bool is_feasible(const std::vector<Order>& orders) {
    if (orders.empty()) return true;

    // Create a copy and sort by Earliest Due Date (d ascending)
    auto sorted = orders;
    std::sort(sorted.begin(), sorted.end(), [](const Order& a, const Order& b) {
        if (a.deadline != b.deadline) return a.deadline < b.deadline;
        return a.original_line < b.original_line;
    });

    uint64_t cumulative_time = 0;
    for (const auto& o : sorted) {
        cumulative_time += o.processing_time;
        if (cumulative_time > o.deadline) {
            return false;
        }
    }
    return true;
}

FeasibilityReport evaluate_feasibility(const std::vector<Order>& orders) {
    FeasibilityReport report;
    if (orders.empty()) {
        report.is_feasible = true;
        return report;
    }

    auto sorted = orders;
    std::sort(sorted.begin(), sorted.end(), [](const Order& a, const Order& b) {
        if (a.deadline != b.deadline) return a.deadline < b.deadline;
        return a.original_line < b.original_line;
    });

    uint64_t cumulative_time = 0;
    for (const auto& o : sorted) {
        cumulative_time += o.processing_time;
        if (cumulative_time > o.deadline) {
            ++report.late_order_count;
            if (report.first_late_job_id.empty()) {
                report.first_late_job_id = o.id;
                report.first_late_completion = cumulative_time;
                report.first_late_deadline = o.deadline;
            }
        }
    }

    report.total_duration = cumulative_time;
    report.is_feasible = (report.late_order_count == 0);
    return report;
}
