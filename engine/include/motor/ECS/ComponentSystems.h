#pragma once

#include <entt/entt.hpp>
#include <motor/ECS/Components.h>
#include <motor/other/ModelLoader.h>


class ComponentSystems 
{
public:
    static void SetPosition(TransformComponent& transform_comp, DirectX::XMFLOAT3 pos);
    static void SetRotation(TransformComponent& transform_comp, DirectX::XMFLOAT3 rot);
    static void SetModel(MeshComponent& mesh_comp, ModelStruct& model);
    static void UpdateBoundingBox(MeshComponent& mesh_comp, TransformComponent& transform_comp, ModelStruct& model);
};