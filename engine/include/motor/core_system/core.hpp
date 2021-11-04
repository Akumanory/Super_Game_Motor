#pragma once

#include <motor/task_system.hpp>
#include <motor/state_system/state.hpp>
#include <motor/ui_system/ui.hpp>
#include <motor/core_system/console.hpp>
#include <motor/core_system/lua.hpp>

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
    sol::state lua_;
    motor::task_system::thread_pool thread_pool_;
    motor::ui_system::CoreUI currentUI_;
    motor::core_system::LuaConsole lua_console_{ lua_ };
};

} // namespace core_system
} // namespace motor