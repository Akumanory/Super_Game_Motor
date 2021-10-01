#pragma once
#include "Mesh.h"

using namespace DirectX;

class Model
{
public:
	bool Initialize(const std::string& file_path, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
	bool InitializeCube(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
	bool InitializeSphere(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
	void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const XMMATRIX& viewProjectionMatrix);
	void DrawMeshes(const XMMATRIX& viewProjectionMatrix);

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	const XMVECTOR& GetLocalPositionVector() const;
	const XMFLOAT3& GetLocalPositionFloat3() const;
	const XMVECTOR& GetLocalRotationVector() const;
	const XMFLOAT3& GetLocalRotationFloat3() const;

	const XMFLOAT3& GetSumPositionFloat3() const;

	void SetPosition(const XMVECTOR& new_pos, bool local = false, bool orbital = false);
	void SetPosition(const XMFLOAT3& new_pos, bool local = false, bool orbital = false);
	void SetPosition(float x, float y, float z, bool local = false, bool orbital = false);

	void AdjustPosition(const XMVECTOR& new_pos, bool local = false, bool orbital = false);
	void AdjustPosition(const XMFLOAT3& new_pos, bool local = false, bool orbital = false);
	void AdjustPosition(float x, float y, float z, bool local = false, bool orbital = false);

	void SetRotation(const XMVECTOR& new_rot, bool local = false, bool orbital = false);
	void SetRotation(const XMFLOAT3& new_rot, bool local = false, bool orbital = false);
	void SetRotation(float x, float y, float z, bool local = false, bool orbital = false);

	void AdjustRotation(const XMVECTOR& new_rot, bool local = false, bool orbital = false);
	void AdjustRotation(const XMFLOAT3& new_rot, bool local = false, bool orbital = false);
	void AdjustRotation(float x, float y, float z, bool local = false, bool orbital = false);

	void SetScale(float x, float y, float z, bool local = false, bool orbital = false);

	void RotateByRadiusAroundY(float theta_speed, float radius, bool local = false, float adjust_x = 0.0f, float adjust_y = 0.0f, float adjust_z = 0.0f);

	void SetLookAtPos(XMFLOAT3 look_at_pos);

	//void RotateByOrbit(
	//	float pos_x, float pos_y, float pos_z,
	//	float rot_x, float rot_y, float rot_z
	//);

	const XMVECTOR& GetForwardVector(bool omit_Y = false);
	const XMVECTOR& GetRightVector(bool omit_Y = false);
	const XMVECTOR& GetBackwardVector(bool omit_Y = false);
	const XMVECTOR& GetLeftVector(bool omit_Y = false);


	void UpdateWorldMatrix(bool orbital);
	void UpdateLocalMatrix(bool orbital);
private:
	std::vector<Mesh> meshes;
	/*bool LoadModel(const std::string& file_path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);*/

	XMFLOAT3 SumPos();

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ConstantBuffer<CB_VS_vertex_shader>* cb_vs_vertexshader = nullptr;
	ID3D11ShaderResourceView* texture = nullptr;

	VertexBuffer<Vertex> vertex_buffer;
	IndexBuffer index_buffer;

	XMMATRIX world_matrix = XMMatrixIdentity();
	XMMATRIX local_matrix = XMMatrixIdentity();

	XMVECTOR pos_vector;
	XMVECTOR rot_vector;
	XMVECTOR scale_vector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;

	// TODO: Добавить методы для этих переменных
	XMVECTOR local_pos_vector;
	XMVECTOR local_rot_vector;
	XMVECTOR local_scale_vector;
	XMFLOAT3 local_pos;
	XMFLOAT3 local_rot;
	XMFLOAT3 local_scale;

	XMFLOAT3 sum_pos;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;

	XMVECTOR vec_forward_no_Y;
	XMVECTOR vec_left_no_Y;
	XMVECTOR vec_right_no_Y;
	XMVECTOR vec_backward_no_Y;
	
	// TODO: Добавить методы для этих переменных
	XMVECTOR local_vec_forward;
	XMVECTOR local_vec_left;
	XMVECTOR local_vec_right;
	XMVECTOR local_vec_backward;

	float theta;
};
