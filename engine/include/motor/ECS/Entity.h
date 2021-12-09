#pragma once

#include <entt/entt.hpp>
#include <motor/ECS/Scene.h>

class Entity
{
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;


	template<typename T>
    T& AddComponent();

	template<typename T>
	T& GetComponent();

	template <typename T>
	bool HasComponent();

	template<typename T>
	void RemoveComponent();


	operator bool() const { return m_entity_handle != entt::null; }
	operator entt::entity() const { return m_entity_handle;}

	bool operator==(const Entity& other) const
	{
		return m_entity_handle == other.m_entity_handle && m_scene == other.m_scene;
	}

	bool operator!=(const Entity& other) const 
	{
		return !(*this == other);
	}

private:
	Scene* m_scene = nullptr;
	entt::entity m_entity_handle {entt::null};
};