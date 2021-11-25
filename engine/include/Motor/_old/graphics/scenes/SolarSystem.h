#pragma once

#include <motor/graphics/Model.h>

class SolarSystem
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader)
	{
		for (size_t i = 0; i < 10; i++)
		{
            models.emplace_back();
			if (!models[i].InitializeCube(device, deviceContext, texture, cb_vs_vertexshader))
			{
				return false;
			}
			models[i].SetPosition(0.0f, 0.0f, 0.0f);
		}
	}

	void AddCube(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader, DirectX::XMFLOAT3 pos) {
        models.emplace_back();
        models.back().InitializeCube(device, deviceContext, texture, cb_vs_vertexshader);
        models.back().SetPosition(pos.x, pos.y, pos.z);
	}

	void Update(float delta)
	{
		//models[0].AdjustRotation(0.0f, 0.001f * delta, 0.0f);

		//// 1 Cluster
		//models[1].AdjustRotation(0.0f, 0.001f * delta, 0.0f);
		//models[1].RotateByRadiusAroundY(0.0003f * delta, 10.0f);
		//
		//models[2].SetPosition(
		//	models[1].GetPositionFloat3().x,
		//	models[1].GetPositionFloat3().y,
		//	models[1].GetPositionFloat3().z);

		//models[2].AdjustRotation(0.0f, -0.001f * delta, 0.0f);
		//models[2].RotateByRadiusAroundY(-0.0003f * delta, 4.0f, true);
		//
		//models[3].SetPosition(
		//	models[2].GetSumPositionFloat3().x,
		//	models[2].GetSumPositionFloat3().y,
		//	models[2].GetSumPositionFloat3().z);
		//models[3].AdjustRotation(0.0f, 0.003f * delta, 0.0f);
		//models[3].RotateByRadiusAroundY(0.0006f * delta, 2.0f, true);

		//// 2 Cluster
		//models[4].AdjustRotation(0.0f, -0.003f * delta, 0.0f);
		//models[4].RotateByRadiusAroundY(-0.0006f * delta, 6.0f);

		//models[5].SetPosition(
		//	models[4].GetPositionFloat3().x,
		//	models[4].GetPositionFloat3().y,
		//	models[4].GetPositionFloat3().z);

		//models[5].AdjustRotation(0.0f, -0.001f * delta, 0.0f);
		//models[5].RotateByRadiusAroundY(0.0009f * delta, 2.0f, true);

		//models[9].SetPosition(
		//	models[4].GetPositionFloat3().x,
		//	models[4].GetPositionFloat3().y,
		//	models[4].GetPositionFloat3().z);

		//models[9].AdjustRotation(0.0f, -0.001f * delta, 0.0f);
		//models[9].RotateByRadiusAroundY(-0.0012f * delta, 3.0f, true);

		//// 3 Cluster
		//models[6].AdjustRotation(0.0f, 0.001f * delta, 0.0f);
		//models[6].RotateByRadiusAroundY(0.0001f * delta, 16.0f);

		//models[7].SetPosition(
		//	models[6].GetPositionFloat3().x,
		//	models[6].GetPositionFloat3().y,
		//	models[6].GetPositionFloat3().z);

		//models[7].AdjustRotation(0.0f, -0.004f * delta, 0.0f);
		//models[7].RotateByRadiusAroundY(-0.0009f * delta, 4.0f, true);

		//models[8].SetPosition(
		//	models[6].GetPositionFloat3().x,
		//	models[6].GetPositionFloat3().y,
		//	models[6].GetPositionFloat3().z);

		//models[8].AdjustRotation(0.0f, -0.004f * delta, 0.0f);
		//models[8].RotateByRadiusAroundY(0.0009f * delta, 3.0f, true);
	}

	void DrawScene(const DirectX::XMMATRIX& viewProjectionMatrix)
	{
		for (size_t i = 0; i < models.size(); i++)
		{
			models[i].Draw(viewProjectionMatrix);
		}
	}

private:
	std::vector<Model> models;
};