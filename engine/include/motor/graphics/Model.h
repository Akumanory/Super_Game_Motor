#pragma once

#include <motor/graphics/Mesh.h>
#include <DirectXCollision.h>

class Model {
public:
    bool Initialize(const std::string& file_path, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
    bool InitializeCube(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
    bool InitializeSphere(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
    void SetTexture(ID3D11ShaderResourceView* texture);
    void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);
    void DrawMeshes(const DirectX::XMMATRIX& viewProjectionMatrix);

    const DirectX::XMVECTOR& GetPositionVector() const;
    const DirectX::XMFLOAT3& GetPositionFloat3() const;
    const DirectX::XMVECTOR& GetRotationVector() const;
    const DirectX::XMFLOAT3& GetRotationFloat3() const;

    const DirectX::XMVECTOR& GetLocalPositionVector() const;
    const DirectX::XMFLOAT3& GetLocalPositionFloat3() const;
    const DirectX::XMVECTOR& GetLocalRotationVector() const;
    const DirectX::XMFLOAT3& GetLocalRotationFloat3() const;

    const DirectX::XMFLOAT3& GetSumPositionFloat3() const;

    void SetPosition(const DirectX::XMVECTOR& new_pos, bool local = false, bool orbital = false);
    void SetPosition(const DirectX::XMFLOAT3& new_pos, bool local = false, bool orbital = false);
    void SetPosition(float x, float y, float z, bool local = false, bool orbital = false);

    void AdjustPosition(const DirectX::XMVECTOR& new_pos, bool local = false, bool orbital = false);
    void AdjustPosition(const DirectX::XMFLOAT3& new_pos, bool local = false, bool orbital = false);
    void AdjustPosition(float x, float y, float z, bool local = false, bool orbital = false);

    void SetRotation(const DirectX::XMVECTOR& new_rot, bool local = false, bool orbital = false);
    void SetRotation(const DirectX::XMFLOAT3& new_rot, bool local = false, bool orbital = false);
    void SetRotation(float x, float y, float z, bool local = false, bool orbital = false);

    void AdjustRotation(const DirectX::XMVECTOR& new_rot, bool local = false, bool orbital = false);
    void AdjustRotation(const DirectX::XMFLOAT3& new_rot, bool local = false, bool orbital = false);
    void AdjustRotation(float x, float y, float z, bool local = false, bool orbital = false);

    void SetScale(float x, float y, float z, bool local = false, bool orbital = false);

    void RotateByRadiusAroundY(float theta_speed, float radius, bool local = false, float adjust_x = 0.0f, float adjust_y = 0.0f, float adjust_z = 0.0f);

    void SetLookAtPos(DirectX::XMFLOAT3 look_at_pos);

    //void RotateByOrbit(
    //	float pos_x, float pos_y, float pos_z,
    //	float rot_x, float rot_y, float rot_z
    //);

    const DirectX::XMVECTOR& GetForwardVector(bool omit_Y = false);
    const DirectX::XMVECTOR& GetRightVector(bool omit_Y = false);
    const DirectX::XMVECTOR& GetBackwardVector(bool omit_Y = false);
    const DirectX::XMVECTOR& GetLeftVector(bool omit_Y = false);

    void UpdateWorldMatrix(bool orbital);
    void UpdateLocalMatrix(bool orbital);

    DirectX::BoundingOrientedBox bounding_box_frustum;

private:
    std::vector<Mesh> meshes;
    /*bool LoadModel(const std::string& file_path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);*/

    DirectX::XMFLOAT3 SumPos();

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    ConstantBuffer<CB_VS_vertex_shader>* cb_vs_vertexshader = nullptr;
    ID3D11ShaderResourceView* texture = nullptr;

    VertexBuffer<Vertex> vertex_buffer;
    IndexBuffer index_buffer;

    DirectX::XMMATRIX world_matrix = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX local_matrix = DirectX::XMMatrixIdentity();

    DirectX::XMVECTOR pos_vector;
    DirectX::XMVECTOR rot_vector;
    DirectX::XMVECTOR scale_vector;
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 rot;
    DirectX::XMFLOAT3 scale;

    // TODO: Добавить методы для этих переменных
    DirectX::XMVECTOR local_pos_vector;
    DirectX::XMVECTOR local_rot_vector;
    DirectX::XMVECTOR local_scale_vector;
    DirectX::XMFLOAT3 local_pos;
    DirectX::XMFLOAT3 local_rot;
    DirectX::XMFLOAT3 local_scale;

    DirectX::XMFLOAT3 sum_pos;

    const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    const DirectX::XMVECTOR DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
    const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
    const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

    DirectX::XMVECTOR vec_forward;
    DirectX::XMVECTOR vec_left;
    DirectX::XMVECTOR vec_right;
    DirectX::XMVECTOR vec_backward;

    DirectX::XMVECTOR vec_forward_no_Y;
    DirectX::XMVECTOR vec_left_no_Y;
    DirectX::XMVECTOR vec_right_no_Y;
    DirectX::XMVECTOR vec_backward_no_Y;

    // TODO: Добавить методы для этих переменных
    DirectX::XMVECTOR local_vec_forward;
    DirectX::XMVECTOR local_vec_left;
    DirectX::XMVECTOR local_vec_right;
    DirectX::XMVECTOR local_vec_backward;

    float theta;
};
