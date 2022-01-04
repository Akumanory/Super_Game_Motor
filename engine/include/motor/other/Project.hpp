#pragma once

#include <rapidjson/document.h>

#include <filesystem>

namespace motor {

namespace rj = rapidjson;

class Project {
public:
    struct SceneDesc {
        std::string name;
        std::filesystem::path path;
        bool isMainScene{ false };
        bool isIncluded{ false };

        static auto from_json(rj::Value& obj) -> SceneDesc;
        auto to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void;
    };
    struct ProjectConfig {
        std::string name;
        std::vector<SceneDesc> scenes;

        static auto from_json(rj::Value& obj) -> ProjectConfig;
        auto to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void;
    };

public:
    Project();
    Project(Project const&) = delete;
    Project(Project&&) = delete;
    Project& operator=(Project const&) = delete;
    Project& operator=(Project&&) = delete;
    ~Project() = default;

    static auto GetProjectDir() -> std::filesystem::path;
    auto SetProjectDir(std::filesystem::path path) -> void;
    auto OpenProject(std::filesystem::path path) -> void;
    auto CreateProject(std::string name, std::filesystem::path path) -> void;
    auto CloseProject() -> void;
    auto IsOpened() const -> bool;
    auto SaveScene(std::string name) const -> void;
    auto LoadScene(std::string name) const -> void;
    auto GetScenes() const -> std::vector<SceneDesc>;
    auto CreateScene(std::string name) -> void;
    auto DeleteScene(std::string name) -> void;
    auto SetMainScene(std::string name) -> void;
    auto SetIncludeScene(std::string name, bool include) -> void;

private:
    auto FindScene_(std::string const& name) -> std::vector<SceneDesc>::iterator;
    auto FindScene_(std::string const& name) const -> std::vector<SceneDesc>::const_iterator;

private:
    static Project* project_;
    std::filesystem::path projectDir_;
    ProjectConfig projectConfig_;
    bool opened_{ false };
};

} // namespace motor
