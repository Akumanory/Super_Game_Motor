#include <motor/core_system/core.hpp>

namespace motor {
namespace core_system {

core::core() {
    lua_.open_libraries(sol::lib::base);
}

void core::run() {
}

void core::load_() {
}

void core::run_loop_() {
}

} // namespace core_system
} // namespace motor