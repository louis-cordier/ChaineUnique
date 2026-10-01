#pragma once

#include "order.hpp"
#include <vector>
#include <string>

/**
 * @brief Parses an order book text file.
 * 
 * Each line must contain: ID processing_time deadline penalty
 * Separated by whitespace.
 * 
 * Empty lines and lines starting with '#' are ignored.
 * The original line number in the file is recorded for tie-breaking.
 * 
 * @param filepath Path to the order book file.
 * @return std::vector<Order> List of parsed orders.
 * @throws std::runtime_error if file cannot be opened or parsed.
 */
std::vector<Order> parse_order_book_file(const std::string& filepath);
