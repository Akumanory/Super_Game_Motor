#include <motor/graphics/RenderableGameObject.h>

using namespace DirectX;

bool RenderableGameObject::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader)
{
	if (!model.Initialize(filePath, device, deviceContext, cb_vs_vertexshader))
		return false;
	

	this->SetPosition(0.0f, 0.0f, 0.0f);
	this->SetRotation(0.0f, 0.0f, 0.0f);

	bounding_box = model.GetBoundingBox();

	this->UpdateMatrix();
	return true;
}

void RenderableGameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	bounding_sphere = BoundingSphere(XMFLOAT3(pos.x, pos.y, pos.z), 1.0f);
	model.Draw(this->worldMatrix, viewProjectionMatrix);
}

DirectX::XMMATRIX RenderableGameObject::GetWorldMatrix() 
{ 
	return worldMatrix;
}

void RenderableGameObject::UpdateMatrix()
{
	this->worldMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
	this->UpdateDirectionVectors();
	bounding_box.Transform(bounding_box, worldMatrix);
}