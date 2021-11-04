#pragma once

#include <motor/core_system/lua.hpp>

#include <vector>
#include <string>

namespace motor {
namespace core_system {

class LuaConsole {
public:
    LuaConsole(sol::state& lua);
    LuaConsole(LuaConsole const&) = delete;
    LuaConsole(LuaConsole&&) = delete;
    LuaConsole& operator=(LuaConsole const&) = delete;
    LuaConsole& operator=(LuaConsole&&) = delete;
    ~LuaConsole();

public:
    static LuaConsole* getInstance();
    bool input(std::string_view command);
    std::vector<std::string>& getLog();

private:
    static int l_print_(lua_State*);
    static void luaopen_l_print_(lua_State*);

private:
    std::vector<std::string> log_;
    int history_iterator_{ -1 };
    sol::state* lua_{ nullptr };
    static inline LuaConsole* instance_{ nullptr };
    std::string input_cache_;
};

} // namespace core_system
} // namespace motor