#pragma once

#include <entt/entt.hpp>

#include <filesystem>

namespace motor {
namespace state_system {

class state {
public:
    ~state();

public:
    virtual void load() = 0;
    virtual void save() = 0;
};

class global_state : public state {
public:
    global_state();
    global_state(std::filesystem::path path);

public:
    void load() override;
    void save() override;
    void set_path(std::filesystem::path path);
};

} // namespace state
} // namespace motor