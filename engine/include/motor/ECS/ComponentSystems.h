#pragma once

#include <entt/entt.hpp>
#include <motor/ECS/Components.h>
#include <motor/other/ModelLoader.h>
#include <motor/ECS/Entity.h>


class ComponentSystems 
{
public:
    static void SetPosition(Entity& entity, DirectX::XMFLOAT3 pos);
    static void SetRotation(Entity& entity, DirectX::XMFLOAT3 rot);
    static void AjustRotation(Entity& entity, DirectX::XMFLOAT3 rot, float delta);
    static void SetModel(Entity& entity, ModelStruct& model);

private:
    static void UpdateBoundingBox(Entity& entity);
};