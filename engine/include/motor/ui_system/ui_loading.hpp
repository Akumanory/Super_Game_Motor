#pragma once

#include <motor/ui_system/ui.hpp>

#include <map>
#include <mutex>

namespace motor {
namespace ui_system {

class LoadingUI : public UI {
public:
    void Draw(const char* title, bool* p_open) override {
        if (strs_.empty()) {
            return;
        }
        std::lock_guard lock(mtx_);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration
          | ImGuiWindowFlags_NoDocking
          | ImGuiWindowFlags_AlwaysAutoResize
          | ImGuiWindowFlags_NoSavedSettings
          | ImGuiWindowFlags_NoFocusOnAppearing
          | ImGuiWindowFlags_NoNav
          | ImGuiWindowFlags_NoMove;
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos;
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos{ work_pos.x + PAD, work_pos.y + work_size.y - PAD }, window_pos_pivot{ 0.0f, 1.0f };
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowBgAlpha(0.35f);
        if (ImGui::Begin(title, p_open, window_flags)) {
            ImGui::Text("Loading...");
            ImGui::Separator();
            for (auto& [key, value] : strs_) {
                ImGui::Text(value.c_str());
            }
        }
        ImGui::End();
    }

    int64_t AddStr(std::string_view sv) {
        std::lock_guard lock(mtx_);
        strs_[current_id_].assign(sv);
        return current_id_++;
    }

    void RemoveStr(int64_t id) {
        std::lock_guard lock(mtx_);
        strs_.erase(strs_.find(id));
    }

private:
    std::map<int64_t, std::string> strs_;
    int64_t current_id_{ 0 };
    std::mutex mtx_;
};

} // namespace ui_system
} // namespace motor