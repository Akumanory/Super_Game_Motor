#pragma once

#include <motor/other/Json.hpp>

#include <filesystem>

namespace motor {

namespace rj = rapidjson;

class Config {
public:
    struct ConfigStruct {
        std::filesystem::path lastFolder;
        std::vector<std::tuple<std::u8string, std::filesystem::path>> lastProjects;

        static auto from_json(rj::Value& obj) -> ConfigStruct {
            decltype(lastProjects) lastProjects_v;
            for (auto& v : obj["lastProjects"].GetArray()) {
                lastProjects_v.emplace_back(std::make_tuple(
                  std::u8string{ (char8_t const*)v["name"].GetString() },
                  std::filesystem::path{ v["path"].GetString() }));
            }
            return {
                .lastFolder = { obj["lastFolder"].GetString() },
                .lastProjects = std::move(lastProjects_v)
            };
        }
        auto to_json(rj::Value& obj, rj::Document::AllocatorType& rjAllocator) const -> void {
            rj::Value lastFolder_j{ rj::kStringType };
            lastFolder_j.SetString((char const*)lastFolder.generic_u8string().c_str(), rjAllocator);
            rj::Value lastProjects_j{ rj::kArrayType };
            for (auto& i : lastProjects) {
                rj::Value pair{ rj::kObjectType };
                rj::Value name{ rj::kStringType };
                rj::Value path{ rj::kStringType };
                name.SetString((char const*)std::get<std::u8string>(i).c_str(), rjAllocator);
                path.SetString((char const*)std::get<std::filesystem::path>(i).generic_u8string().c_str(), rjAllocator);
                pair.AddMember("name", name, rjAllocator);
                pair.AddMember("path", path, rjAllocator);
                lastProjects_j.PushBack(pair, rjAllocator);
            }
            obj.AddMember("lastFolder", lastFolder_j, rjAllocator);
            obj.AddMember("lastProjects", lastProjects_j, rjAllocator);
        }
    };

    Config() {
        configPath_ = std::filesystem::absolute(std::filesystem::current_path() / "engine_config.json");
        if (std::filesystem::exists(configPath_)) {
            config_ = json::readFile<ConfigStruct>(configPath_);
        } else {
            ResetLastFolder();
        }
        if (not std::filesystem::exists(config_.lastFolder)) {
            ResetLastFolder();
        }
    }

    void ResetLastFolder() {
        config_.lastFolder = std::filesystem::absolute(std::filesystem::current_path());
        json::writeFile(configPath_, config_);
    }

    ConfigStruct& GetConfig() {
        return config_;
    }

    void Save() {
        json::writeFile(configPath_, config_);
    }

    void AddLastProject(std::u8string name, std::filesystem::path path) {
        auto&& lp = config_.lastProjects;
        auto it = std::find_if(lp.begin(), lp.end(), [&](auto& v) {
            return std::get<std::u8string>(v) == name and std::get<std::filesystem::path>(v) == path;
        });
        if (it != lp.end()) {
            lp.erase(it);
        }
        while (lp.size() >= 10) {
            lp.pop_back();
        }
        lp.emplace(lp.begin(), std::make_tuple(std::move(name), std::move(path)));
        Save();
    }

    void SetLastFolder(std::filesystem::path path) {
        config_.lastFolder = std::move(path);
        Save();
    }

private:
    std::filesystem::path configPath_;
    ConfigStruct config_;
};

} // namespace motor