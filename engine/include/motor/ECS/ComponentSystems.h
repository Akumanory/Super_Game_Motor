#pragma once

#include <motor/ECS/Components.h>
#include <motor/other/ModelLoader.h>

class ComponentSystems 
{
public:
    static void SetPosition(Entity& entity, DirectX::XMFLOAT3 pos);
    static void SetRotation(Entity& entity, DirectX::XMFLOAT3 rot);
    static void AjustRotation(Entity& entity, DirectX::XMFLOAT3 rot, float delta);
    static void AjustPosition(Entity& entity, DirectX::XMFLOAT3 pos, float delta);
    static void AjustPosition(Entity& entity, DirectX::XMVECTOR pos, float delta);
    static void SetModel(Entity& entity, ModelStruct& model);
    static void UpdateBoundingBox(Entity& entity);
    static void UpdateBoundingFrustum(Entity& entity);
    //static void SetChildEntity(Entity parent, Entity& child);
    static DirectX::XMMATRIX GetTransformMatrix(Entity& entity);

private:
    static DirectX::XMMATRIX ParentTransformMatrix(Entity entity);
};