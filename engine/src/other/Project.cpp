#include <motor/other/Project.hpp>
#include <motor/other/Json.hpp>
#include <motor/other/Config.hpp>
#include <motor/converters.hpp>

#include <cmrc/cmrc.hpp>
CMRC_DECLARE(base);

using namespace motor;
namespace fs = std::filesystem;

Project* Project::project_{ nullptr };

Project::Project() {
    project_ = this;
    opened_ = false;
}

auto Project::GetProjectDir() -> fs::path {
    return project_->projectDir_;
}

auto Project::SetProjectDir(fs::path path) -> void {
    projectDir_ = std::move(path);
}

auto Project::OpenProject(fs::path path) -> void {
    if (not fs::exists(path / "config.json")) {
        throw std::runtime_error("Can't open project: config.json not found");
    }
    fs::current_path(path);
    SetProjectDir(path);
    projectConfig_ = json::readFile<ProjectConfig>(path / "config.json");
    if (not fs::exists(path / "scenes")) {
        fs::create_directories(path / "scenes");
    }
    if (not fs::exists(path / "assets")) {
        fs::create_directories(path / "assets");
    }
    config_.AddLastProject(motor::converters::mb_to_u8(projectConfig_.name), std::filesystem::absolute(path));
    opened_ = true;
}

auto Project::CreateProject(std::string name, fs::path path) -> void {
    if (not fs::exists(path)) {
        fs::create_directories(path);
    }
    fs::current_path(path);
    SetProjectDir(path);
    projectConfig_.name = std::move(name);
    json::writeFile(path / "config.json", projectConfig_);
    fs::create_directories(path / "scenes");
    fs::create_directories(path / "assets");
    fs::create_directories(path / "base_assets");

    auto cfs = cmrc::base::get_filesystem();
    const auto iter = [&cfs](auto it, auto& curdir) {
        auto iter_impl = [&cfs](auto it, auto& curdir, auto& iter_ref) mutable -> void {
            for (auto entry : it) {
                if (entry.is_file()) {
                    auto file = cfs.open((curdir / entry.filename()).generic_string());
                    fs::create_directories("base_assets" / curdir);
                    std::ofstream fout{ "base_assets" / curdir / entry.filename(), std::ios_base::binary };
                    fout.write(file.cbegin(), file.size());
                } else if (entry.is_directory()) {
                    auto dir = curdir / entry.filename();
                    iter_ref(cfs.iterate_directory(dir.generic_string()), dir, iter_ref);
                }
            }
        };
        return iter_impl(it, curdir, iter_impl);
    };
    fs::path p{};
    iter(cfs.iterate_directory(""), p);

    config_.AddLastProject(motor::converters::mb_to_u8(projectConfig_.name), std::filesystem::absolute(path));
    opened_ = true;
}

auto Project::CloseProject() -> void {
    json::writeFile("config.json", projectConfig_);
    opened_ = false;
    projectDir_ = fs::path{};
}

auto Project::IsOpened() const -> bool {
    return opened_;
}

auto Project::SaveScene(std::string name) const -> void {
    auto it = FindScene_(name);
}

auto Project::LoadScene(std::string name) const -> void {
    auto it = FindScene_(name);
}

auto Project::GetScenes() const -> std::vector<SceneDesc> {
    return projectConfig_.scenes;
}

auto Project::CreateScene(std::string name) -> void {
    auto it = std::find_if(projectConfig_.scenes.begin(), projectConfig_.scenes.end(), [&name](auto& scene) {
        return scene.name == name;
    });
    if (it != projectConfig_.scenes.end()) {
        throw std::runtime_error(std::format("Scene {} already exists", name));
    }
    projectConfig_.scenes.emplace_back(SceneDesc{
      .name = name,
      .path = fs::path{ name },
      .isMainScene = false,
      .isIncluded = true });
    fs::create_directory("scenes" / fs::path{ name });
}

auto Project::DeleteScene(std::string name) -> void {
    auto it = FindScene_(name);
    if (fs::exists(it->path)) {
        fs::remove_all(it->path);
    }
    projectConfig_.scenes.erase(it);
}

auto Project::SetMainScene(std::string name) -> void {
    auto it = FindScene_(name);
    it->isMainScene = true;
    for (auto& i : projectConfig_.scenes) {
        i.isMainScene = false;
    }
}

auto Project::SetIncludeScene(std::string name, bool include) -> void {
    auto it = FindScene_(name);
    it->isIncluded = include;
}

auto Project::FindScene_(std::string const& name) -> std::vector<SceneDesc>::iterator {
    auto it = std::find_if(projectConfig_.scenes.begin(), projectConfig_.scenes.end(), [&name](auto& scene) {
        return scene.name == name;
    });
    if (it == projectConfig_.scenes.end()) {
        throw std::runtime_error(std::format("Scene {} not found", name));
    }
    return it;
}

auto Project::FindScene_(std::string const& name) const -> std::vector<SceneDesc>::const_iterator {
    auto it = std::find_if(projectConfig_.scenes.begin(), projectConfig_.scenes.end(), [&name](auto& scene) {
        return scene.name == name;
    });
    if (it == projectConfig_.scenes.end()) {
        throw std::runtime_error(std::format("Scene {} not found", name));
    }
    return it;
}

auto Project::GetEngineConfig() -> Config& {
    return config_;
}

auto Project::ProjectConfig::from_json(rj::Value& obj) -> ProjectConfig {
    std::vector<SceneDesc> scenes;
    auto& scenes_j = obj["scenes"];
    for (auto& v : scenes_j.GetArray()) {
        scenes.emplace_back(SceneDesc::from_json(v));
    }
    return {
        .name = { obj["name"].GetString() },
        .scenes = { std::move(scenes) }
    };
}

auto Project::ProjectConfig::to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void {
    rj::Value name_j{ rj::kStringType };
    name_j.SetString(name.c_str(), name.length(), rjAllocator);
    rj::Value scenes_j{ rj::kArrayType };
    for (auto& i : scenes) {
        rj::Value scene_j{ rj::kObjectType };
        i.to_json(scene_j, rjAllocator);
        scenes_j.PushBack(scene_j, rjAllocator);
    }
    obj.AddMember("name", name_j, rjAllocator);
    obj.AddMember("scenes", scenes_j, rjAllocator);
}

auto Project::SceneDesc::from_json(rj::Value& obj) -> SceneDesc {
    return {
        .name = obj["name"].GetString(),
        .path = fs::path{ obj["path"].GetString() },
        .isMainScene = obj["isMainScene"].GetBool(),
        .isIncluded = obj["isIncluded"].GetBool()
    };
}

auto Project::SceneDesc::to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void {
    rj::Value name_j{ rj::kStringType };
    name_j.SetString(name.c_str(), name.length(), rjAllocator);
    rj::Value path_j{ rj::kStringType };
    auto path_str = path.generic_u8string();
    path_j.SetString((const char*)path_str.c_str(), path_str.length(), rjAllocator);
    rj::Value isMainScene_j;
    isMainScene_j.SetBool(isMainScene);
    rj::Value isIncluded_j;
    isIncluded_j.SetBool(isIncluded);
    obj.AddMember("name", name_j, rjAllocator);
    obj.AddMember("path", path_j, rjAllocator);
    obj.AddMember("isMainScene", isMainScene_j, rjAllocator);
    obj.AddMember("isIncluded", isIncluded_j, rjAllocator);
}