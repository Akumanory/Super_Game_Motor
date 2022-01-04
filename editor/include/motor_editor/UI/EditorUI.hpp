#pragma once

#include <motor/ui_system/ui.hpp>

namespace motor_editor {
namespace UI {

class EditorUI : public motor::ui_system::UI {
public:
    EditorUI() = default;
    EditorUI(EditorUI const&) = delete;
    EditorUI(EditorUI&&) = default;
    EditorUI& operator=(EditorUI const&) = delete;
    EditorUI& operator=(EditorUI&&) = default;

    void Draw(const char* title, bool* p_open) override { 
        if (ImGui::Begin(title, p_open)) {
            ImGui::Text("Hello editor");
            ImGui::End();
        }
        ImGui::ShowDemoWindow();
    }

private:
};

} // namespace UI
} // namespace motor_editor