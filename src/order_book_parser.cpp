#include "order_book_parser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<Order> parse_order_book_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::vector<Order> orders;
    std::string line;
    size_t line_number = 0;

    while (std::getline(file, line)) {
        ++line_number;

        // Trim leading and trailing whitespace
        size_t first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            continue; // Skip empty line
        }

        // Check for comment line
        if (line[first] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string id;
        uint64_t p = 0;
        uint64_t d = 0;
        uint64_t w = 0;

        if (!(iss >> id >> p >> d >> w)) {
            throw std::runtime_error("Parsing error at " + filepath + ":" + 
                                     std::to_string(line_number) + " - expected format: <ID> <p> <d> <w>");
        }

        if (p == 0 || d == 0 || w == 0) {
            throw std::runtime_error("Invalid value at " + filepath + ":" + 
                                     std::to_string(line_number) + " - p, d, and w must be strictly positive integers");
        }

        orders.push_back(Order{id, p, d, w, line_number});
    }

    return orders;
}
