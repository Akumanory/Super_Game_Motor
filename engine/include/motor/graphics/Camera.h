#pragma once

#include <motor/graphics/GameObject.h>

#include <DirectXMath.h>

class Camera : public GameObject
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const DirectX::XMMATRIX& GetViewMatrix() const;
	const DirectX::XMMATRIX& GetProjectionMatrix() const;

private:
	void UpdateMatrix() override;

	DirectX::XMMATRIX view_matrix;
	DirectX::XMMATRIX projection_matrix;
};
