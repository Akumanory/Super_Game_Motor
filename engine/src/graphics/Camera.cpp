#include <motor/graphics/Camera.h>

using namespace DirectX;

Camera::Camera()
{
    _name = "NULL";
	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);

	this->UpdateMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	projection_matrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
    DirectX::BoundingFrustum::CreateFromMatrix(f_culling, projection_matrix);
}

std::string Camera::GetCameraName() const
{
	return _name;
}

void Camera::SetCameraName(std::string name) 
{
    _name = name;
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return view_matrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	return projection_matrix;
}

const DirectX::BoundingFrustum& Camera::GetLocalBoundingFrustum() const {
    return local_f_culling;
}

void Camera::UpdateMatrix() //Updates view matrix and also updates the movement vectors
{
	//Calculate camera rotation matrix
	XMMATRIX cam_rotation_matrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	//Calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR cam_target = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, cam_rotation_matrix);
	//Adjust cam target to be offset by the camera's current position
	cam_target += this->posVector;
	//Calculate up direction based on current rotation
	XMVECTOR up_direction = XMVector3TransformCoord(DEFAULT_UP_VECTOR, cam_rotation_matrix);
	//Rebuild view matrix
	view_matrix = XMMatrixLookAtLH(this->posVector, cam_target, up_direction);

	XMVECTOR Det = XMMatrixDeterminant(view_matrix);
    XMMATRIX invView = XMMatrixInverse(&Det, view_matrix);

	f_culling.Transform(local_f_culling, invView);

	UpdateDirectionVectors();
}