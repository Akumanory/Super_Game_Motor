#pragma once

#include <motor/other/Display.h>
#include <motor/graphics/Graphics.h>
#include <motor/other/Timer.h>
#include <motor/ui_system/ui_console.hpp>
#include <motor/core_system/console.hpp>
#include <motor/core_system/lua.hpp>

class Framework
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_class, int width, int height);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
private:
	Timer timer;
	Display display;
	Graphics gfx;

	sol::state lua_;
    motor::ui_system::ConsoleUI consoleUI_;
    motor::LuaConsole lua_console_{ lua_ };
    bool showConsole_{ true };
};