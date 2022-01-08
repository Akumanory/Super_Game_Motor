#pragma once

#include <motor/task_system.hpp>
#include <motor/state_system/state.hpp>
#include <motor/ui_system/ui.hpp>
#include <motor/ui_system/ui_console.hpp>
#include <motor/core_system/logic.hpp>
#include <motor/ECS/Scene.h>
#include <motor/core_system/display.hpp>
#include <motor/core_system/input_device.hpp>
#include <motor/other/Timer.h>
#include <motor/graphics/Graphics.h>

#include <concepts>

namespace motor {

class core {
public:
    core(std::string_view title, HINSTANCE hInstance);
    core(core const&) = delete;
    core(core&&) = delete;
    core& operator=(core const&) = delete;
    core& operator=(core&&) = delete;
    ~core() = default;

public:
    auto run() -> void;

    template <std::derived_from<ui_system::UI> UI>
    auto setUI() {
        currentUI_ = std::make_unique<UI>();
    }

private:
    auto drawUI_() -> void;
    auto drawScene_() -> void;

private:
    motor::task_system::thread_pool thread_pool_;
    Logic logic_;
    Timer timer_;
    InputDevice inputDevice_;
    Display display_;
    Graphics gfx_;
    std::unique_ptr<ui_system::UI> currentUI_;
    std::unique_ptr<Scene> currentScene_;
    //motor::ui_system::ConsoleUI consoleUI_;
    //bool showConsole_{ false };
    bool showUI_{ true };
};

} // namespace motor