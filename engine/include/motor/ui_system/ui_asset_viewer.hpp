#pragma once

#include <motor/ui_system/ui.hpp>
#include <imfilebrowser/imfilebrowser.hpp>
#include <motor/utils.hpp>
#include <motor/other/Project.hpp>
#include <motor/converters.hpp>

#include <filesystem>

namespace motor {
namespace ui_system {

class AssetViewerUI : public UI {
public:
    AssetViewerUI() {
        fileDialog_.SetTitle("Select files to import");
        fileDialogFolders_.SetTitle("Select folders to import");
        fileDialogWhere_.SetTitle("Choose where to import");
    }

    void Draw(const char* title, bool* p_open) override {
        namespace fs = std::filesystem;

        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        if (ImGui::Button("Import files")) {
            fileDialog_.Open();
        }
        if (ImGui::Button("Import folders")) {
            fileDialogFolders_.Open();
        }

        fileDialog_.Display();
        fileDialogFolders_.Display();
        fileDialogWhere_.Display();

        if (fileDialog_.HasSelected()) {
            ff_ = fileDialog_.GetMultiSelected();
            fileDialog_.ClearSelected();

            utils::debug_write::info("Selected files:\n");
            for (auto& f : ff_) {
                utils::debug_write::info("    {}\n", converters::u8_to_mb(f.generic_u8string()));
            }

            fileDialogWhere_.SetPwd("assets");
            fileDialogWhere_.Open();
        }
        if (fileDialogFolders_.HasSelected()) {
            ff_ = fileDialogFolders_.GetMultiSelected();
            fileDialogFolders_.ClearSelected();

            utils::debug_write::info("Selected folders:\n");
            for (auto& f : ff_) {
                utils::debug_write::info("    {}\n", converters::u8_to_mb(f.generic_u8string()));
            }

            fileDialogWhere_.SetPwd("assets");
            fileDialogWhere_.Open();
        }
        if (fileDialogWhere_.HasSelected()) {
            where_ = fileDialogWhere_.GetSelected();
            fileDialogWhere_.ClearSelected();

            if (not is_nested_("assets", where_)) {
                utils::debug_write::info("This directory is outside!\n");
            } else {
                utils::debug_write::info("Import to folder {}\n", converters::u8_to_mb(where_.generic_u8string()));

                for (auto& f : ff_) {
                    fs::copy(f, where_,
                      fs::copy_options::overwrite_existing | fs::copy_options::recursive);
                }
            }
        }

        if (not root_.empty()) {
            if (current_ != root_) {
                if (ImGui::Button("..")) {
                    current_ = current_.parent_path();
                }
            }
            for (auto& entry : fs::directory_iterator{ current_ }) {
                if (entry.is_directory()) {
                    if (ImGui::Button((const char*)entry.path().filename().generic_u8string().c_str())) {
                        current_ = entry.path();
                    }
                } else {
                    ImGui::Text((const char*)entry.path().filename().generic_u8string().c_str());
                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                        dragdrop_ = entry.path();
                        strcpy_s(dragdropName_, (const char*)dragdrop_.filename().generic_u8string().c_str());
                        ImGui::SetDragDropPayload("Asset##Payload", &dragdropName_, strlen(dragdropName_), ImGuiCond_Once);
                        ImGui::Text("%s", ImGui::GetDragDropPayload()->Data);
                        ImGui::EndDragDropSource();
                    }
                }
            }
        }

        ImGui::End();
    }

    void SetRoot(std::filesystem::path root) {
        root_ = root;
        current_ = root;
    }

    std::filesystem::path GetDragDrop() {
        return dragdrop_;
    }

private:
    bool is_nested_(std::filesystem::path const& root, std::filesystem::path const& path) {
        auto final_root = std::filesystem::absolute(root).lexically_normal();
        auto final_path = std::filesystem::absolute(path).lexically_normal();
        auto [root_end, nothing] = std::mismatch(final_root.begin(), final_root.end(), final_path.begin());
        return root_end == final_root.end();
    }

private:
    ImGui::FileBrowser fileDialog_{ ImGuiFileBrowserFlags_MultipleSelection };
    ImGui::FileBrowser fileDialogFolders_{ ImGuiFileBrowserFlags_MultipleSelection | ImGuiFileBrowserFlags_SelectDirectory };
    ImGui::FileBrowser fileDialogWhere_{ ImGuiFileBrowserFlags_SelectDirectory | ImGuiFileBrowserFlags_CreateNewDir };
    std::vector<std::filesystem::path> ff_;
    std::filesystem::path where_;
    std::filesystem::path current_;
    std::filesystem::path root_;
    std::filesystem::path dragdrop_;
    char dragdropName_[256]{};
};

} // namespace ui_system
} // namespace motor