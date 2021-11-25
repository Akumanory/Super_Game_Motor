#include <motor/core_system/core.hpp>

extern lua_State* lState;
void LoadImguiBindings();

namespace motor {
namespace core_system {

core::core() {
    lState = lua_.lua_state();
    lua_.open_libraries(sol::lib::base, sol::lib::jit);
    LoadImguiBindings();
}

void core::run() {
    //run_loop_();
}

void core::load_() {
}

void core::run_loop_() {
    while (true) {
        if (showUI_) {
            currentUI_.Draw("UI", &showUI_);
        }
        if (showConsole_) {
            consoleUI_.Draw("Console", &showConsole_);
        }
    }
}

} // namespace core_system
} // namespace motor