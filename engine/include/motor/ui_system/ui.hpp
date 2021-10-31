#pragma once

namespace motor {
namespace ui_system {

class ui {
public:
    virtual void draw() = 0;
};

class core_ui : public ui {
    void draw() override { }
};

} // namespace ui_system
} // namespace motor