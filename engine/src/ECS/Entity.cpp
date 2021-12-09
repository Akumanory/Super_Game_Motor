#include <motor/ECS/Entity.h>
#include <assert.h>

Entity::Entity(entt::entity handle, Scene* scene)
    : m_entity_handle(handle)
    , m_scene(scene) {
}
