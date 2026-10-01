#pragma once

#include "../order.hpp"
#include <vector>
#include <iostream>

/**
 * @brief Renders an ASCII timeline ("la frise") or tabular view.
 * 
 * Per project specifications:
 * - If total cumulative processing time <= 60 (or if force_timeline is true):
 *   Prints a 1-character-per-time-unit compact ASCII timeline with deadline indicators.
 * - Otherwise:
 *   Switches automatically to a clean tabular display.
 * 
 * @param orders The list of orders in execution order.
 * @param os Output stream (defaults to std::cout).
 * @param force_timeline If true, force ASCII timeline display regardless of cumulative time.
 */
void render_schedule_timeline(const std::vector<Order>& orders, std::ostream& os = std::cout, bool force_timeline = false);
