#pragma once

#include <filesystem>

namespace motor {

extern std::filesystem::path ProjectDir;

std::filesystem::path GetProjectDir();
void SetProjectDir(std::filesystem::path);

class AssetManager {
public:
private:
};

} // namespace motor