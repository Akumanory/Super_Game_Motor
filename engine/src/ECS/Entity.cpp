#include <motor/ECS/Entity.h>


Entity::Entity(entt::entity handle, Scene* scene)
    : m_entity_handle(handle)
    , m_scene(scene) {
}
