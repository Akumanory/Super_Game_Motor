#pragma once

#include <cstdint>
#include <chrono>

namespace motor {
namespace task_system {

using chrono_clock = std::chrono::high_resolution_clock;

constexpr int32_t const MAX_THREADS_COUNT{ 32 };
constexpr double const MIN_TIMER_RATIO{ 0.0001 };
constexpr double const MAX_TIMER_RATIO{ 10000.0 };
constexpr double const INIT_TIMER_RATIO{ 1. };

} // namespace task_system
} // namespace motor