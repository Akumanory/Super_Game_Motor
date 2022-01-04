#pragma once

#include <motor/ui_system/ui.hpp>
#include <motor/other/Project.hpp>
#include <imfilebrowser/imfilebrowser.hpp>

namespace motor {
namespace ui_system {

class ProjectUI : UI {
public:
    ProjectUI(Project& project)
        : project_{ project } {
        fileBrowser_.SetTitle("Open project");
        fileBrowserCreate_.SetTitle("Create project");
    }
    ProjectUI(ProjectUI const&) = delete;
    ProjectUI(ProjectUI&&) = default;
    ProjectUI& operator=(ProjectUI const&) = delete;
    ProjectUI& operator=(ProjectUI&&) = default;
    ~ProjectUI() = default;

    void Draw(const char* title, bool* p_open) override {
        ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Always);
        if (!ImGui::BeginPopupModal(title, p_open, ImGuiWindowFlags_NoSavedSettings)) {
            return;
        }

        if (ImGui::Button("Open")) {
            fileBrowser_.Open();
        }
        if (ImGui::Button("Create")) {
            fileBrowserCreate_.Open();
        }

        fileBrowser_.Display();
        fileBrowserCreate_.Display();

        if (fileBrowser_.HasSelected()) {
            auto projPath = fileBrowser_.GetSelected();
            fileBrowser_.ClearSelected();

            utils::debug_write::info("Open project: {} ...\n", projPath.generic_string());
            project_.OpenProject(std::move(projPath));
            ImGui::CloseCurrentPopup();
            *p_open = false;
        }
        if (fileBrowserCreate_.HasSelected()) {
            auto projPath = fileBrowserCreate_.GetSelected();
            fileBrowserCreate_.ClearSelected();

            if (!std::filesystem::is_empty(projPath)) {
                ImGui::OpenPopup("Not empty");
            } else {
                auto projName = projPath.filename().generic_string();
                utils::debug_write::info("Create project with name {} and path {} ...\n", projName, projPath.generic_string());
                project_.CreateProject(std::move(projName), std::move(projPath));
                ImGui::CloseCurrentPopup();
                *p_open = false;
            }
        }

        ImGui::SetNextWindowSize(ImVec2(250, 150), ImGuiCond_Always);
        if (ImGui::BeginPopupModal("Not empty", nullptr, ImGuiWindowFlags_NoSavedSettings)) {
            ImGui::Text("This directory is not empty");
            if (ImGui::Button("OK")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::EndPopup();
    }

private:
    Project& project_;
    ImGui::FileBrowser fileBrowser_{ ImGuiFileBrowserFlags_SelectDirectory };
    ImGui::FileBrowser fileBrowserCreate_{ ImGuiFileBrowserFlags_SelectDirectory | ImGuiFileBrowserFlags_CreateNewDir };
};

} // namespace ui_system
} // namespace motor