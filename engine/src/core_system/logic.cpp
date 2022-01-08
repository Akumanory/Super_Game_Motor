#include <motor/core_system/logic.hpp>

extern lua_State* lState;
void LoadImguiBindings();

using namespace motor;

Logic::Logic() {
    lState = lua_.lua_state();
    lua_.open_libraries(
      sol::lib::base,
      sol::lib::package,
      sol::lib::math,
      sol::lib::string,
      sol::lib::table,
      sol::lib::debug,
      sol::lib::jit);
    LoadImguiBindings();
}