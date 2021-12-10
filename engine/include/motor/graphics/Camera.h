#pragma once

#include <motor/graphics/GameObject.h>

#include <DirectXMath.h>

class Camera : public GameObject
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
	std::string GetCameraName() const;
    void SetCameraName(std::string name);
	const DirectX::XMMATRIX& GetViewMatrix() const;
	const DirectX::XMMATRIX& GetProjectionMatrix() const;
	const DirectX::BoundingFrustum& GetLocalBoundingFrustum() const;

private:
	void UpdateMatrix() override;
    std::string _name = "NULL";
	DirectX::XMMATRIX view_matrix;
	DirectX::XMMATRIX projection_matrix;
	DirectX::BoundingFrustum local_f_culling;
	DirectX::BoundingFrustum f_culling;
};
