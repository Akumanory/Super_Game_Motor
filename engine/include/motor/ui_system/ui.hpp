#pragma once

#include <imgui.h>

namespace motor {
namespace ui_system {

class UI {
public:
    virtual ~UI() = default;
    virtual void Draw(const char* title, bool* p_open) = 0;
};

class CoreUI : public UI {
public:
    void Draw(const char* title, bool* p_open) override { }
};

} // namespace ui_system
} // namespace motor