#include <motor/ECS/Factory.h>

entt::entity SimpleGameObject(entt::registry& reg) 
{
    const entt::entity e = reg.create();
    reg.emplace<PositionRotation>(e);
    reg.emplace<ObjectModel>(e);
    return e;
}

entt::entity TestNewGameObject(entt::registry& reg) 
{
    const entt::entity e = reg.create();
    reg.emplace<TagComponent>(e);
    reg.emplace<TransformComponent>(e);
    reg.emplace<MeshComponent>(e);

    return e;
}