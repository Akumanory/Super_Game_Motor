#pragma once

#include <motor/graphics/UpdatedModel.h>


struct PositionRotation
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMMATRIX worldMatrix;

    void UpdateMatrix() 
    {
        worldMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * 
                      DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    }
};


struct ObjectModel
{
    UpdatedModel model;
};