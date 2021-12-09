#pragma once

#include <entt/entt.hpp>
#include <motor/ECS/Scene.h>

class Entity {
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;

    template <typename T>
    T& AddComponent() {
        assert(!HasComponent<T>() && "Entity already has component");
        T& component = m_scene->m_registry.emplace<T>(m_entity_handle);
        return component;
    }

    template <typename T>
    T& GetComponent() {
        assert(HasComponent<T>() && "Entity does not have component");
        return m_scene->m_registry.get<T>(m_entity_handle);
    }

    template <typename T>
    bool HasComponent() {
        return m_scene->m_registry.any_of<T>(m_entity_handle);
    }

    template <typename T>
    void RemoveComponent() {
        assert(HasComponent<T>() && "Entity does not have component");
        return m_scene->m_registry.remove<T>(m_entity_handle);
    }

    operator bool() const { return m_entity_handle != entt::null; }
    operator entt::entity() const { return m_entity_handle; }

    bool operator==(const Entity& other) const {
        return m_entity_handle == other.m_entity_handle && m_scene == other.m_scene;
    }

    bool operator!=(const Entity& other) const {
        return !(*this == other);
    }

private:
    Scene* m_scene = nullptr;
    entt::entity m_entity_handle{ entt::null };
};