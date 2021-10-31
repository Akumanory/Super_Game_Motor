#pragma once

#include <motor/task_system.hpp>
#include <motor/state_system/state.hpp>
#include <motor/ui_system/ui.hpp>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace motor {
namespace core_system {

class core {
public:
    core();

public:
    void run();

private:
    void load_();
    void run_loop_();

private:
    motor::task_system::thread_pool thread_pool_;
    motor::ui_system::core_ui current_ui_;
};

} // namespace core_system
} // namespace motor