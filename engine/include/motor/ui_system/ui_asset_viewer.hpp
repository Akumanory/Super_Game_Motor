#pragma once

#include <motor/ui_system/ui.hpp>
#include <imfilebrowser/imfilebrowser.hpp>
#include <motor/utils.hpp>

#include <filesystem>

namespace motor {
namespace ui_system {

class AssetViewerUI : public UI {
public:
    AssetViewerUI(std::filesystem::path root)
        : root_{ std::move(root) } { }

    void Draw(const char* title, bool* p_open) override {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        if (ImGui::Button("Import")) {
            fileDialog_.Open();
        }

        fileDialog_.Display();

        if (fileDialog_.HasSelected()) {
            utils::debug_write::info("Selected filename: {}\n", fileDialog_.GetSelected().generic_string());
            fileDialog_.ClearSelected();
        }

        ImGui::End();
    }

private:
    std::filesystem::path root_;
    ImGui::FileBrowser fileDialog_;
};

} // namespace ui_system
} // namespace motor