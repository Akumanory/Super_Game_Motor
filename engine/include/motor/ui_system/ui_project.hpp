#pragma once

#include <motor/ui_system/ui.hpp>
#include <motor/other/Project.hpp>
#include <imfilebrowser/imfilebrowser.hpp>
#include <motor/converters.hpp>

namespace motor {
namespace ui_system {

class ProjectUI : UI {
public:
    ProjectUI(Project& project)
        : project_{ project } {
        fileBrowser_.SetTitle("Open project");
        fileBrowser_.SetPwd(project_.GetEngineConfig().GetConfig().lastFolder);
        fileBrowserCreate_.SetTitle("Create project");
        fileBrowserCreate_.SetPwd(project_.GetEngineConfig().GetConfig().lastFolder);
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
        ImGui::Separator();
        ImGui::Text("Last projects:");
        for (auto& i : project_.GetEngineConfig().GetConfig().lastProjects) {
            if (ImGui::Button((char const*)std::get<std::u8string>(i).c_str())) {
                project_.OpenProject(std::get<std::filesystem::path>(i));
                *p_open = false;
            }
            ImGui::SameLine();
            ImGui::Text((char const*)std::get<std::filesystem::path>(i).generic_u8string().c_str());
        }

        fileBrowser_.Display();
        fileBrowserCreate_.Display();

        if (fileBrowser_.HasSelected()) {
            project_.GetEngineConfig().SetLastFolder(fileBrowser_.GetPwd());
            fileBrowserCreate_.SetPwd(fileBrowser_.GetPwd());
            auto projPath = fileBrowser_.GetSelected();
            fileBrowser_.ClearSelected();

            utils::debug_write::info("Open project: {} ...\n", converters::u8_to_mb(projPath.generic_u8string()));
            project_.OpenProject(std::move(projPath));
            ImGui::CloseCurrentPopup();
            *p_open = false;
        }
        if (fileBrowserCreate_.HasSelected()) {
            project_.GetEngineConfig().SetLastFolder(fileBrowserCreate_.GetPwd());
            fileBrowser_.SetPwd(fileBrowserCreate_.GetPwd());
            auto projPath = fileBrowserCreate_.GetSelected();
            fileBrowserCreate_.ClearSelected();

            if (!std::filesystem::is_empty(projPath)) {
                ImGui::OpenPopup("Not empty");
            } else {
                auto projName = converters::u8_to_mb(projPath.filename().generic_u8string());
                utils::debug_write::info("Create project with name {} and path {} ...\n", projName, converters::u8_to_mb(projPath.generic_u8string()));
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