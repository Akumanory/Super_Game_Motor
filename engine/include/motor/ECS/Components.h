#pragma once

#include <motor/graphics/UpdatedModel.h>
#include <motor/graphics/MeshForComponents.h>
#include <motor/other/ModelLoader.h>

using namespace DirectX;

class Entity;

// Component Done
struct TagComponent 
{
    std::string tag = "Undefined Tag";
};

// Component Done
struct TransformComponent 
{
    XMFLOAT3 world_position = { 0.0, 0.0, 0.0 };
    XMFLOAT3 world_rotation = { 0.0, 0.0, 0.0 };
    
    XMFLOAT3 local_position = { 0.0, 0.0, 0.0 };
    XMFLOAT3 local_rotation = { 0.0, 0.0, 0.0 };
    XMFLOAT3 local_scale    = { 1.0, 1.0, 1.0 };

    // translation * rotation * scale
    XMMATRIX GetWorldTransformMatrix() const 
    {
        return XMMatrixRotationRollPitchYaw(world_rotation.x, world_rotation.y, world_rotation.z) *
               XMMatrixTranslation(world_position.x, world_position.y, world_position.z);
    }

    XMMATRIX GetLocalTransformMatrix() const {
        return XMMatrixScaling(local_scale.x, local_scale.y, local_scale.z) * 
               XMMatrixRotationRollPitchYaw(local_rotation.x, local_rotation.y, local_rotation.z) * 
               XMMatrixTranslation(local_position.x, local_position.y, local_position.z) * 
               GetWorldTransformMatrix();
    }
};

// Undone
//struct DirectionalVectorsComponent 
//{
//    const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//    const XMVECTOR DEFAULT_UP_VECTOR      = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//    const XMVECTOR DEFAULT_RIGHT_VECTOR   = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//
//    // Todo: UpdateDirectionalVectors
//};

// TODO: Undone
struct MeshComponent 
{
    BoundingOrientedBox transformed_bounding_box;
    ModelStruct model;
};

// Компонент указывает на то что entity является родлителем для данных entity
struct ChildsComponent 
{
    std::vector<Entity> child_entities;
};

// Коспонет указывает на то что entity является чьим то ребенком
struct ParentComponent 
{
    Entity* parent;
};

//struct CameraComponent
//{
//    SceneCamera camera;
//    bool primary;
//};