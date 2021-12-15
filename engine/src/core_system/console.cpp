#include <motor/core_system/console.hpp>

using namespace motor;

LuaConsole::LuaConsole(sol::state& lua)
    : lua_{ &lua } {
    instance_ = this;
    lua_->open_libraries(sol::lib::base);
    luaopen_l_print_(lua.lua_state());
}

LuaConsole::~LuaConsole() {
    instance_ = nullptr;
}

LuaConsole* LuaConsole::getInstance() {
    return instance_;
}

bool LuaConsole::input(std::string_view command) {
    log_.emplace_back(std::string(input_cache_.empty() ? "> " : ">> ") + command.data());
    auto res = lua_->script(
      input_cache_.empty()
        ? command
        : input_cache_ + "\n" + command.data());
    if (res.status() == sol::call_status::syntax) {
        sol::error error = res;
        std::string_view what{ error.what() };
        if (what.ends_with("'<eof>'")) {
            input_cache_ += std::string("\n") + command.data();
        } else {
            log_.emplace_back(std::string("[error] ") + what.data());
            input_cache_.clear();
        }
    } else {
        input_cache_.clear();
    }
    return not input_cache_.empty();
}

std::vector<std::string>& LuaConsole::getLog() {
    return log_;
}

int LuaConsole::l_print_(lua_State* L) {
    int nargs = lua_gettop(L);

    for (int i = 1; i <= nargs; i++) {
        //if (lua_isstring(L, i)) {
        /* Pop the next arg using lua_tostring(L, i) and do your print */
        const char* msg = lua_tostring(L, i);
        if (msg != nullptr) {
            //utils::debug_write::info("str = {}\n", msg);
            LuaConsole::getInstance()->log_.emplace_back(msg);
        } else {
            LuaConsole::getInstance()->log_.emplace_back("--- bad lua output ---");
        }
        //} else {
        //    /* Do something with non-strings if you like */
        //}
    }
    return 0;
};

void LuaConsole::luaopen_l_print_(lua_State* L) {
    static const struct luaL_Reg printlib[] = {
        { "print", l_print_ },
        { NULL, NULL }
    };

    lua_getglobal(L, "_G");
    // luaL_register(L, NULL, printlib); // for Lua versions < 5.2
    luaL_setfuncs(L, printlib, 0); // for Lua versions 5.2 or greater
    lua_pop(L, 1);
}
