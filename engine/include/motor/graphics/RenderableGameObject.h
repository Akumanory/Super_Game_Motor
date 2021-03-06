#pragma once

#include <motor/graphics/GameObject.h>

class RenderableGameObject : public GameObject
{
public:
	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);

	void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);

	DirectX::XMMATRIX GetWorldMatrix();

protected:
	UpdatedModel model;
	void UpdateMatrix() override;

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
};
