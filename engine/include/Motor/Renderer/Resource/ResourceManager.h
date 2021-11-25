#pragma once

#include <Motor/Renderer/Prerequisites.h>
#include <Motor/Renderer/Resource/Resource.h>

#include <unordered_map>
#include <string>

namespace motor {
class ResourceManager {
public:
    ResourceManager();
    virtual ~ResourceManager();

    ResourcePtr createResourceFromFile(const wchar_t* file_path);

protected:
    virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path) = 0;

private:
    std::unordered_map<std::wstring, ResourcePtr> m_map_resources;
};
} // namespace motor
