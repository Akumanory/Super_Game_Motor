#include <motor/ECS/Entity.h>
#include <assert.h>

Entity::Entity(entt::entity handle, Scene* scene)
    : m_entity_handle(handle)
    , m_scene(scene) 
{ 
}

template<typename T>
T& Entity::AddComponent() 
{
    assert(!HasComponent<T>() && "Entity already has component");
    T& component = m_scene->m_registry.emplace<T>(m_entity_handle);
    return component;
}

template <typename T>
T& Entity::GetComponent()
{
    assert(HasComponent<T>() && "Entity does not have component");
    return m_scene->m_registry.get<T>(m_entity_handle);
}

template <typename T>
void Entity::RemoveComponent() 
{
    assert(HasComponent<T>() && "Entity does not have component");
    return m_scene->m_registry.remove<T>(m_entity_handle);
}

template <typename T>
bool Entity::HasComponent() 
{
    return m_scene->m_registry.any_of<T>(m_entity_handle);
}