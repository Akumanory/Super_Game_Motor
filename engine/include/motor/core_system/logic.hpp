#pragma once

#include <motor/core_system/console.hpp>
#include <motor/core_system/lua.hpp>

namespace motor {

class Logic {
public:
    Logic();
    Logic(Logic const&) = delete;
    Logic(Logic&&) = default;
    Logic& operator=(Logic const&) = delete;
    Logic& operator=(Logic&&) = default;
    ~Logic() = default;

public:
    void run();

private:
    sol::state lua_;
    motor::LuaConsole lua_console_{ lua_ };
};

} // namespace motor