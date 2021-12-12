#pragma once

#include <motor/graphics/UpdatedModel.h>
#include <motor/graphics/MeshForComponents.h>
#include <motor/other/ModelLoader.h>

using namespace DirectX;

// New Components

// Component Done
struct TagComponent 
{
    std::string tag = "Undefined Tag";
};

// Component Done
struct TransformComponent 
{
    XMFLOAT3 position = { 0.0, 0.0, 0.0 };
    XMFLOAT3 rotation = { 0.0, 0.0, 0.0 };
    XMFLOAT3 scale    = { 1.0, 1.0, 1.0 };

    // Отдельно матрицы rotation, translation, scale
    // .....
    XMMATRIX GetScaleMatrix() const {

        return XMMatrixScaling(scale.x, scale.y, scale.z);
    }

    XMMATRIX GetRotationMatrix() const 
    {
        return XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    }

    XMMATRIX GetTranslationMatrix() const {

        return XMMatrixTranslation(position.x, position.y, position.z);
    }
    // .....

    // translation * rotation * scale
    XMMATRIX GetTransformMatrix() const 
    {
        return XMMatrixScaling(scale.x, scale.y, scale.z) *
               XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
               XMMatrixTranslation(position.x, position.y, position.z);
    }
};

// Undone
struct DirectionalVectorsComponent 
{
    const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

    // Todo: UpdateDirectionalVectors

};

// TODO: Undone
struct MeshComponent 
{
    BoundingOrientedBox transformed_bounding_box;
    ModelStruct model;
};
