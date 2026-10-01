#include "ascii_timeline.hpp"
#include <iomanip>
#include <sstream>

void render_schedule_timeline(const std::vector<Order>& orders, std::ostream& os, bool force_timeline) {
    uint64_t total_p = 0;
    for (const auto& o : orders) {
        total_p += o.processing_time;
    }

    if (orders.empty()) {
        os << "(No orders to display)\n";
        return;
    }

    // --- CASE 1: Cumulative duration <= 60 (or forced) -> Compact ASCII Timeline ---
    if (total_p <= 60 || force_timeline) {
        os << "\n========================================================================\n"
           << "                 ASCII SCHEDULE TIMELINE (Cumulative <= 60)\n"
           << "========================================================================\n";

        // 1. Build machine execution timeline string
        std::string machine_line;
        machine_line.reserve(total_p + 10);

        uint64_t current_time = 0;
        for (const auto& o : orders) {
            uint64_t p = o.processing_time;
            if (p == 1) {
                machine_line += "|";
            } else if (p == 2) {
                machine_line += "[]";
            } else {
                std::string block = "[" + o.id.substr(0, std::min<size_t>(o.id.size(), p - 2));
                while (block.size() < p - 1) {
                    block += "-";
                }
                block += "]";
                machine_line += block;
            }
            current_time += p;
        }

        // 2. Build time ruler
        std::string ruler_ticks = "0";
        for (size_t t = 1; t <= total_p; ++t) {
            if (t % 10 == 0) {
                ruler_ticks += std::to_string(t % 100);
                if (t >= 10 && t % 100 >= 10) {
                    // Two digits used, so we don't need extra char
                }
            } else if (t % 5 == 0) {
                ruler_ticks += "+";
            } else {
                ruler_ticks += ".";
            }
        }
        // Ensure ruler matches total_p length
        if (ruler_ticks.size() > total_p + 1) {
            ruler_ticks = ruler_ticks.substr(0, total_p + 1);
        }
        while (ruler_ticks.size() <= total_p) {
            ruler_ticks += ".";
        }

        os << "Time : " << ruler_ticks << "\n";
        os << "Mach : " << machine_line << "\n\n";

        // 3. Execution breakdown
        os << std::left
           << std::setw(6)  << "Line"
           << std::setw(12) << "ID"
           << std::setw(8)  << "Start"
           << std::setw(8)  << "End"
           << std::setw(10) << "Deadline"
           << std::setw(12) << "Status"
           << std::setw(10) << "Penalty"
           << "\n";
        os << std::string(64, '-') << "\n";

        uint64_t t = 0;
        uint64_t total_late_penalties = 0;
        size_t on_time_count = 0;

        for (const auto& o : orders) {
            uint64_t start = t;
            uint64_t end = t + o.processing_time;
            bool on_time = (end <= o.deadline);
            if (on_time) {
                ++on_time_count;
            } else {
                total_late_penalties += o.penalty;
            }

            os << std::left
               << std::setw(6)  << o.original_line
               << std::setw(12) << o.id
               << std::setw(8)  << start
               << std::setw(8)  << end
               << std::setw(10) << o.deadline
               << std::setw(12) << (on_time ? "ON TIME" : "LATE")
               << std::setw(10) << (on_time ? 0 : o.penalty)
               << "\n";

            t = end;
        }
        os << std::string(64, '-') << "\n";
        os << "Result: " << on_time_count << "/" << orders.size() 
           << " orders on time | Late penalties due: " << total_late_penalties << " EUR\n";
        os << "========================================================================\n\n";

    } else {
        // --- CASE 2: Cumulative duration > 60 -> Tabular View ---
        os << "\n========================================================================\n"
           << "          TABULAR VIEW (Cumulative duration > 60: " << total_p << " units)\n"
           << "========================================================================\n";

        os << std::left
           << std::setw(6)  << "Line"
           << std::setw(14) << "ID"
           << std::setw(10) << "p (time)"
           << std::setw(12) << "d (deadline)"
           << std::setw(12) << "w (penalty)"
           << std::setw(10) << "Ratio (w/p)"
           << "\n";
        os << std::string(64, '-') << "\n";

        size_t preview_limit = std::min<size_t>(orders.size(), 20);
        for (size_t i = 0; i < preview_limit; ++i) {
            const auto& o = orders[i];
            os << std::left
               << std::setw(6)  << o.original_line
               << std::setw(14) << o.id
               << std::setw(10) << o.processing_time
               << std::setw(12) << o.deadline
               << std::setw(12) << o.penalty
               << std::fixed << std::setprecision(2)
               << std::setw(10) << o.ratio()
               << "\n";
        }
        if (orders.size() > preview_limit) {
            os << "... (" << (orders.size() - preview_limit) << " more orders omitted) ...\n";
        }
        os << "========================================================================\n\n";
    }
}
