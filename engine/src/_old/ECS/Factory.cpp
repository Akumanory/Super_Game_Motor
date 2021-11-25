#include <motor/ECS/Factory.h>

entt::entity SimpleGameObject(entt::registry& reg) 
{
    const entt::entity e = reg.create();
    reg.emplace<PositionRotation>(e);
    reg.emplace<ObjectModel>(e);
    return e;
}