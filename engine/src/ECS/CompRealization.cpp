#include <motor/ECS/CompRealization.h>

void SetPosition(PositionRotation& pos_struct, DirectX::XMFLOAT3 posVector) {
    pos_struct.position = posVector;
    pos_struct.UpdateMatrix();
}

void SetRotation(PositionRotation& rot_struct, DirectX::XMFLOAT3 rotVector) {
    rot_struct.rotation = rotVector;
    rot_struct.UpdateMatrix();
}

void SetModel(ObjectModel& model_struct, const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader) 
{
    model_struct.model.Initialize(filePath, device, deviceContext, cb_vs_vertexshader);
}