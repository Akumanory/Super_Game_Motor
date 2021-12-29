#pragma once

#include <motor/other/Display.h>
#include <motor/graphics/Graphics.h>
#include <motor/other/Timer.h>
#include <motor/ui_system/ui_console.hpp>
#include <motor/ui_system/ui_asset_viewer.hpp>
#include <motor/core_system/console.hpp>
#include <motor/core_system/lua.hpp>
#include <motor/ECS/Scene.h>
#include <motor/task_system.hpp>


class Framework
{
public:
    Framework() = default;
	~Framework() {
        tpool_.get()->stop();
	}

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
    motor::ui_system::AssetViewerUI assetViewerUI_{ std::filesystem::current_path() };
    bool showAssetViewer_{ true };

	std::unique_ptr<motor::task_system::thread_pool> tpool_;
};