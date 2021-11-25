#pragma once
#include <motor/ECS/Components.h>
#include <motor/ECS/Factory.h>
#include <motor/ECS/CompRealization.h>
#include <entt/entt.hpp>

class Scene
{
public:
    Scene();

private:
    entt::registry m_registry;
};