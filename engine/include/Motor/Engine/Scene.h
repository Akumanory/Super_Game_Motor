#pragma once

namespace motor {
class Entity;
class Scene {
public:
    Scene() { }
    virtual ~Scene() { }

    virtual Entity* createEntity() = 0;
};
} // namespace motor
