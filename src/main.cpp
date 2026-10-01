#include "order.hpp"
#include "order_book_parser.hpp"
#include "generator/order_generator.hpp"
#include "visualizer/ascii_timeline.hpp"
#include "scheduler/feasibility_judge.hpp"
#include <iostream>
#include <iomanip>
#include <string>

void print_help(const char* prog_name) {
    std::cout << "Single-Machine Job Scheduling (Chaine Unique) - Engine v1.0\n\n"
              << "Usage:\n"
              << "  " << prog_name << " [options] [order_book_file]\n\n"
              << "General Options:\n"
              << "  -h, --help               Show this help message\n"
              << "  -f, --file PATH          Path to order book file (.txt) to load\n"
              << "  -j, --judge              Run the Feasibility Judge (Jackson / EDD rule)\n"
              << "  -t, --timeline           Force ASCII timeline display\n\n"
              << "Generator Options (Milestone 1):\n"
              << "  -g, --generate           Generate a new order book with planted solution\n"
              << "  -s, --seed NUM           Random seed (default: 42)\n"
              << "  -p, --planted NUM        Number of planted feasible orders (default: 10)\n"
              << "  -n, --noise NUM          Number of noise orders with tight deadlines (default: 5)\n"
              << "  -o, --out PATH           Output file path for generated book (default: carnets/generated.txt)\n"
              << "  -w, --witness PATH       Output file path for external witness benchmark (default: carnets/witness.txt)\n\n"
              << "Examples:\n"
              << "  " << prog_name << " carnets/sample.txt\n"
              << "  " << prog_name << " --judge carnets/sample.txt\n"
              << "  " << prog_name << " --generate --seed 12345 --planted 8 --noise 4\n";
}

int main(int argc, char* argv[]) {
    std::string filepath;
    bool mode_generate = false;
    bool force_timeline = false;
    bool run_judge = false;

    GeneratorConfig gen_config;
    std::string out_filepath = "carnets/generated.txt";
    std::string witness_filepath = "carnets/witness.txt";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_help(argv[0]);
            return 0;
        } else if (arg == "-g" || arg == "--generate") {
            mode_generate = true;
        } else if (arg == "-j" || arg == "--judge") {
            run_judge = true;
        } else if (arg == "-t" || arg == "--timeline") {
            force_timeline = true;
        } else if ((arg == "-f" || arg == "--file") && i + 1 < argc) {
            filepath = argv[++i];
        } else if ((arg == "-s" || arg == "--seed") && i + 1 < argc) {
            gen_config.seed = static_cast<uint32_t>(std::stoul(argv[++i]));
        } else if ((arg == "-p" || arg == "--planted") && i + 1 < argc) {
            gen_config.num_planted = static_cast<size_t>(std::stoul(argv[++i]));
        } else if ((arg == "-n" || arg == "--noise") && i + 1 < argc) {
            gen_config.num_noise = static_cast<size_t>(std::stoul(argv[++i]));
        } else if ((arg == "-o" || arg == "--out") && i + 1 < argc) {
            out_filepath = argv[++i];
        } else if ((arg == "-w" || arg == "--witness") && i + 1 < argc) {
            witness_filepath = argv[++i];
        } else if (arg[0] != '-') {
            filepath = arg;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            print_help(argv[0]);
            return 1;
        }
    }

    // --- EXECUTION MODE 1: Generator ---
    if (mode_generate) {
        std::cout << "\n========================================\n"
                  << "        ORDER BOOK GENERATOR\n"
                  << "========================================\n"
                  << " PRNG Algorithm  : Xorshift32\n"
                  << " Seed            : " << gen_config.seed << "\n"
                  << " Planted Orders  : " << gen_config.num_planted << " (guaranteed on time)\n"
                  << " Noise Orders    : " << gen_config.num_noise << " (tight deadlines)\n"
                  << " Output File     : " << out_filepath << "\n"
                  << " Witness File    : " << witness_filepath << "\n"
                  << "========================================\n\n";

        try {
            auto result = generate_order_book(gen_config);
            save_order_book(out_filepath, result.orders);
            save_witness_file(witness_filepath, result.planted_count, result.seed);

            std::cout << "Successfully generated " << result.orders.size() << " orders.\n"
                      << "Order book saved to: " << out_filepath << "\n"
                      << "External witness saved to: " << witness_filepath << "\n\n";

            // Run judge if requested
            if (run_judge) {
                auto report = evaluate_feasibility(result.orders);
                std::cout << "Feasibility Judge verdict: "
                          << (report.is_feasible ? "FEASIBLE (100% on time)" : "INFEASIBLE (Late orders detected)")
                          << "\n";
            }

            render_schedule_timeline(result.orders, std::cout, force_timeline);
            return 0;

        } catch (const std::exception& ex) {
            std::cerr << "Generation error: " << ex.what() << "\n";
            return 1;
        }
    }

    // --- EXECUTION MODE 2: Load and Process Order Book ---
    if (filepath.empty()) {
        std::cout << "Single-Machine Job Scheduling (Chaine Unique) - Engine v1.0\n"
                  << "No input file or action specified. Run with --help for usage.\n";
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
                  << "========================================\n";

        // Feasibility Judge Evaluation
        auto report = evaluate_feasibility(orders);
        std::cout << "\n----------------------------------------\n"
                  << "    FEASIBILITY JUDGE (Jackson / EDD)\n"
                  << "----------------------------------------\n";
        if (report.is_feasible) {
            std::cout << " Status: [FEASIBLE] - All " << orders.size() 
                      << " orders can be delivered 100% on time without delay!\n";
        } else {
            std::cout << " Status: [INFEASIBLE] - Cannot deliver all orders on time.\n"
                      << " Late orders in EDD schedule : " << report.late_order_count << "\n"
                      << " First late order detected   : " << report.first_late_job_id 
                      << " (finishes at t=" << report.first_late_completion 
                      << ", deadline was d=" << report.first_late_deadline << ")\n";
        }
        std::cout << "----------------------------------------\n\n";

        // Display ASCII timeline or tabular view
        render_schedule_timeline(orders, std::cout, force_timeline);

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
