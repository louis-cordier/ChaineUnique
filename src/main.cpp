#include "order.hpp"
#include "order_book_parser.hpp"
#include <iostream>
#include <iomanip>
#include <numeric>

void print_help(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " [options] [order_book_file]\n\n"
              << "Options:\n"
              << "  -h, --help       Show this help message\n"
              << "  -f, --file PATH  Path to order book file (.txt)\n\n"
              << "Example:\n"
              << "  " << prog_name << " carnets/sample.txt\n";
}

int main(int argc, char* argv[]) {
    std::string filepath;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_help(argv[0]);
            return 0;
        } else if ((arg == "-f" || arg == "--file") && i + 1 < argc) {
            filepath = argv[++i];
        } else if (arg[0] != '-') {
            filepath = arg;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            print_help(argv[0]);
            return 1;
        }
    }

    if (filepath.empty()) {
        std::cout << "Single-Machine Job Scheduling (Chaine Unique) - Engine v0.1\n";
        std::cout << "No input file specified. Run with --help for usage.\n";
        return 0;
    }

    try {
        std::cout << "Loading order book from: " << filepath << " ...\n";
        std::vector<Order> orders = parse_order_book_file(filepath);

        uint64_t total_p = 0;
        uint64_t total_w = 0;
        for (const auto& o : orders) {
            total_p += o.processing_time;
            total_w += o.penalty;
        }

        std::cout << "\n========================================\n"
                  << "        ORDER BOOK SUMMARY\n"
                  << "========================================\n"
                  << " Total orders parsed : " << orders.size() << "\n"
                  << " Total duration (p)  : " << total_p << " time units\n"
                  << " Total penalties (w) : " << total_w << " EUR\n"
                  << "========================================\n\n";

        std::cout << std::left 
                  << std::setw(6)  << "Line"
                  << std::setw(12) << "ID"
                  << std::setw(10) << "p (time)"
                  << std::setw(12) << "d (deadline)"
                  << std::setw(12) << "w (penalty)"
                  << std::setw(10) << "Ratio (w/p)"
                  << "\n";
        std::cout << std::string(62, '-') << "\n";

        size_t preview_limit = std::min(orders.size(), size_t(10));
        for (size_t i = 0; i < preview_limit; ++i) {
            const auto& o = orders[i];
            std::cout << std::left 
                      << std::setw(6)  << o.original_line
                      << std::setw(12) << o.id
                      << std::setw(10) << o.processing_time
                      << std::setw(12) << o.deadline
                      << std::setw(12) << o.penalty
                      << std::fixed << std::setprecision(2)
                      << std::setw(10) << o.ratio()
                      << "\n";
        }

        if (orders.size() > preview_limit) {
            std::cout << "... (" << (orders.size() - preview_limit) << " more orders omitted) ...\n";
        }
        std::cout << std::string(62, '-') << "\n";

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
