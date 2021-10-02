#pragma once

#include <motor/graphics/UpdatedModel.h>

class RenderableGameObject;

class GameObject
{
public:
    const DirectX::XMVECTOR& GetPositionVector() const;
    const DirectX::XMFLOAT3& GetPositionFloat3() const;
    const DirectX::XMVECTOR& GetRotationVector() const;
    const DirectX::XMFLOAT3& GetRotationFloat3() const;

	void SetPosition(const DirectX::XMVECTOR& pos);
    void SetPosition(const DirectX::XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);
    void AdjustPosition(const DirectX::XMVECTOR& pos);
	void AdjustPosition(const DirectX::XMFLOAT3& pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const DirectX::XMVECTOR& rot);
	void SetRotation(const DirectX::XMFLOAT3& rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const DirectX::XMVECTOR& rot);
	void AdjustRotation(const DirectX::XMFLOAT3& rot);
	void AdjustRotation(float x, float y, float z);
	void SetLookAtPos(DirectX::XMFLOAT3 lookAtPos);

	bool CollisionIntersects(DirectX::BoundingSphere& bound);
	DirectX::BoundingSphere& GetCollisionSphere();
	void BoundToIntersected(GameObject& object, DirectX::XMFLOAT3 offset);
	void DisableCollision();
	void BindOnIntersect(GameObject& child);

	const DirectX::XMVECTOR& GetForwardVector(bool omitY = false);
	const DirectX::XMVECTOR& GetRightVector(bool omitY = false);
	const DirectX::XMVECTOR& GetBackwardVector(bool omitY = false);
	const DirectX::XMVECTOR& GetLeftVector(bool omitY = false);

protected:
	virtual void UpdateMatrix();
	void UpdateDirectionVectors();

	DirectX::BoundingSphere bounding_sphere;

	DirectX::XMVECTOR posVector;
	DirectX::XMVECTOR rotVector;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;

	const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR vec_forward;
	DirectX::XMVECTOR vec_left;
	DirectX::XMVECTOR vec_right;
	DirectX::XMVECTOR vec_backward;

	DirectX::XMVECTOR vec_forward_noY;
	DirectX::XMVECTOR vec_left_noY;
	DirectX::XMVECTOR vec_right_noY;
	DirectX::XMVECTOR vec_backward_noY;

	bool collide = true;
        DirectX::XMFLOAT3 offset;
};
