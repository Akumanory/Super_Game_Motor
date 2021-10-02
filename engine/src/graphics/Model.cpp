#include <motor/graphics/Model.h>

using namespace DirectX;

bool Model::Initialize(const std::string& file_path, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11ShaderResourceView* _texture, ConstantBuffer<CB_VS_vertex_shader>& _cb_vs_vertexshader)
{
	device = _device;
	deviceContext = _deviceContext;
	texture = _texture;
	cb_vs_vertexshader = &_cb_vs_vertexshader;

	try
	{
		/*if (!LoadModel(file_path))
		{
			return false;
		}*/
	}
	catch (COMException ex)
	{
		Logs::Error(ex);
		return false;
	}

	return true;
}

bool Model::InitializeCube(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11ShaderResourceView* _texture, ConstantBuffer<CB_VS_vertex_shader>& _cb_vs_vertexshader)
{
	device = _device;
	deviceContext = _deviceContext;
	texture = _texture;
	cb_vs_vertexshader = &_cb_vs_vertexshader;
	theta = 0;

	try
	{
		//Textured Square
		Vertex v[] =
		{
			Vertex(-0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  -1.0f, -1.0f), //FRONT Bottom Left   - [0]
			Vertex(-0.5f,   0.5f, -0.5f, 0.0f, 0.0f,  -1.0f,   1.0f, -1.0f), //FRONT Top Left      - [1]
			Vertex( 0.5f,   0.5f, -0.5f, 1.0f, 0.0f,  1.0f,   1.0f, -1.0f), //FRONT Top Right      - [2]
			Vertex( 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,  1.0f,  -1.0f, -1.0f), //FRONT Bottom Right   - [3]
			Vertex(-0.5f,  -0.5f,  0.5f, 0.0f, 1.0f,  -1.0f,  -1.0f,  1.0f), //BACK Bottom Left     - [4]
			Vertex(-0.5f,   0.5f,  0.5f, 0.0f, 0.0f,  -0.5f,   0.5f,  1.0f), //BACK Top Left        - [5]
			Vertex( 0.5f,   0.5f,  0.5f, 1.0f, 0.0f,  1.0f,   1.0f,  1.0f), //BACK Top Right        - [6]
			Vertex( 0.5f,  -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,  -1.0f,  1.0f), //BACK Bottom Right     - [7]
		};

		//Load Vertex Data
		HRESULT hr = vertex_buffer.Initialize(device, v, ARRAYSIZE(v));
		COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer.");

		DWORD indices[] =
		{
			0, 1, 2, //FRONT
			0, 2, 3, //FRONT
			4, 7, 6, //BACK 
			4, 6, 5, //BACK
			3, 2, 6, //RIGHT SIDE
			3, 6, 7, //RIGHT SIDE
			4, 5, 1, //LEFT SIDE
			4, 1, 0, //LEFT SIDE
			1, 5, 6, //TOP
			1, 6, 2, //TOP
			0, 3, 7, //BOTTOM
			0, 7, 4, //BOTTOM
		};

		//Load Index Data
		hr = index_buffer.Initialize(device, indices, ARRAYSIZE(indices));
		COM_ERROR_IF_FAILED(hr, "Failed to initialize index buffer.");
	}
	catch (COMException& exception)
	{
		Logs::Error(exception);
		return false;
	}

	// world matrix
	SetPosition(0.0f, 0.0f, 0.0f, false);
	SetRotation(0.0f, 0.0f, 0.0f, false);
	SetScale(1.0f, 1.0f, 1.0f);

	// local matrix
	SetPosition(0.0f, 0.0f, 0.0f, true);
	SetRotation(0.0f, 0.0f, 0.0f, true);
	SetScale(1.0f, 1.0f, 1.0f, true);

	UpdateWorldMatrix(false);
	return true;
}

bool Model::InitializeSphere(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader)
{


	return false;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture)
{
	this->texture = texture;
}

void Model::Draw(const XMMATRIX& viewProjectionMatrix)
{
	//Update Constant buffer with WVP Matrix
	cb_vs_vertexshader->data.wvpMatrix = world_matrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
	cb_vs_vertexshader->data.worldMatrix = world_matrix;
	//cb_vs_vertexshader->data.wvpMatrix = XMMatrixTranspose(cb_vs_vertexshader->data.wvpMatrix);
	//cb_vs_vertexshader->data.worldMatrix = XMMatrixTranspose(cb_vs_vertexshader->data.worldMatrix);
	cb_vs_vertexshader->ApplyChanges();
	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexshader->GetAddressOf());

	deviceContext->PSSetShaderResources(0, 1, &texture); //Set Texture
	deviceContext->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), vertex_buffer.StridePtr(), &offset);
	deviceContext->DrawIndexed(index_buffer.IndexCount(), 0, 0); //Draw
}

void Model::DrawMeshes(const XMMATRIX& viewProjectionMatrix)
{
	//Update Constant buffer with WVP Matrix
	cb_vs_vertexshader->data.wvpMatrix = world_matrix * viewProjectionMatrix; //Calculate World-View-Projection Matrix
	cb_vs_vertexshader->data.worldMatrix = world_matrix;
	//cb_vs_vertexshader->data.wvpMatrix = XMMatrixTranspose(cb_vs_vertexshader->data.wvpMatrix);
	//cb_vs_vertexshader->data.worldMatrix = XMMatrixTranspose(cb_vs_vertexshader->data.worldMatrix);
	cb_vs_vertexshader->ApplyChanges();
	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexshader->GetAddressOf());

	deviceContext->PSSetShaderResources(0, 1, &texture); //Set Texture

	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw();
	}
}

void Model::UpdateWorldMatrix(bool orbital)
{
	SumPos();
	if (orbital)
	{
		world_matrix = XMMatrixTranslation(pos.x, pos.y, pos.z) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * local_matrix;
	}
	else
	{
		world_matrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x, pos.y, pos.z)  * local_matrix;
	}

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.0f);
	vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

	XMMATRIX vecRotationMatrix_no_Y = XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
	vec_forward_no_Y = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix_no_Y);
	vec_backward_no_Y = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix_no_Y);
	vec_left_no_Y = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix_no_Y);
	vec_right_no_Y = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix_no_Y);

}

void Model::UpdateLocalMatrix(bool orbital)
{
	if (orbital)
	{
		local_matrix = XMMatrixTranslation(local_pos.x, local_pos.y, local_pos.z) * XMMatrixRotationRollPitchYaw(local_rot.x, local_rot.y, local_rot.z);
	}
	else
	{
		local_matrix = XMMatrixRotationRollPitchYaw(local_rot.x, local_rot.y, local_rot.z) * XMMatrixTranslation(local_pos.x, local_pos.y, local_pos.z);
	}

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, local_rot.y, 0.0f);
	local_vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	local_vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	local_vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	local_vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

//bool Model::LoadModel(const std::string& file_path)
//{
//	Assimp::Importer importer;
//
//	const aiScene* p_scene = importer.ReadFile(
//		file_path,
//		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
//	);
//
//	if (p_scene == nullptr)
//	{
//		return false;
//	}
//
//	ProcessNode(p_scene->mRootNode, p_scene);
//
//	return true;
//}
//
//void Model::ProcessNode(aiNode* node, const aiScene* scene)
//{
//	for (UINT i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		meshes.push_back(ProcessMesh(mesh, scene));
//	}
//
//	for (UINT i = 0; i < node->mNumChildren; i++)
//	{
//		this->ProcessNode(node->mChildren[i], scene);
//	}
//}
//
//Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
//{
//	// Data to fill
//	std::vector<Vertex> vertices;
//	std::vector<DWORD> indices;
//
//	//Get vertices
//	for (UINT i = 0; i < mesh->mNumVertices; i++)
//	{
//		Vertex vertex;
//
//		vertex.pos.x = mesh->mVertices[i].x;
//		vertex.pos.y = mesh->mVertices[i].y;
//		vertex.pos.z = mesh->mVertices[i].z;
//
//		vertex.normal.x = mesh->mNormals[i].x;
//		vertex.normal.y = mesh->mNormals[i].y;
//		vertex.normal.z = mesh->mNormals[i].z;
//
//		if (mesh->mTextureCoords[0])
//		{
//			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
//			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
//		}
//
//		vertices.push_back(vertex);
//	}
//
//	//Get indices
//	for (UINT i = 0; i < mesh->mNumFaces; i++)
//	{
//		aiFace face = mesh->mFaces[i];
//
//		for (UINT j = 0; j < face.mNumIndices; j++)
//			indices.push_back(face.mIndices[j]);
//	}
//
//	return Mesh(device, deviceContext, vertices, indices);
//}

XMFLOAT3 Model::SumPos()
{
	sum_pos.x = local_pos.x + pos.x;
	sum_pos.y = local_pos.y + pos.y;
	sum_pos.z = local_pos.z + pos.z;

	return sum_pos;
}

const XMVECTOR& Model::GetPositionVector() const
{
	return pos_vector;
}

const XMFLOAT3& Model::GetPositionFloat3() const
{
	return pos;
}

const XMVECTOR& Model::GetRotationVector() const
{
	return rot_vector;
}

const XMFLOAT3& Model::GetRotationFloat3() const
{
	return rot;
}

const XMVECTOR& Model::GetLocalPositionVector() const
{
	return local_pos_vector;
}

const XMFLOAT3& Model::GetLocalPositionFloat3() const
{
	return local_pos;
}

const XMVECTOR& Model::GetLocalRotationVector() const
{
	return local_rot_vector;
}

const XMFLOAT3& Model::GetLocalRotationFloat3() const
{
	return local_rot;
}

const XMFLOAT3& Model::GetSumPositionFloat3() const
{
	return sum_pos;
}

void Model::SetPosition(const XMVECTOR& new_pos, bool local, bool orbital)
{
	
	if (local)
	{
		XMStoreFloat3(&local_pos, new_pos);
		local_pos_vector = new_pos;
		UpdateLocalMatrix(orbital);
	}
	else
	{
		XMStoreFloat3(&pos, new_pos);
		pos_vector = new_pos;
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::SetPosition(const XMFLOAT3& new_pos, bool local, bool orbital)
{
	if (local)
	{
		local_pos = new_pos;
		local_pos_vector = XMLoadFloat3(&local_pos);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		pos = new_pos;
		pos_vector = XMLoadFloat3(&pos);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::SetPosition(float x, float y, float z, bool local, bool orbital)
{
	if (local)
	{
		local_pos = XMFLOAT3(x, y, z);
		local_pos_vector = XMLoadFloat3(&pos);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		pos = XMFLOAT3(x, y, z);
		pos_vector = XMLoadFloat3(&pos);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::AdjustPosition(const XMVECTOR& new_pos, bool local, bool orbital)
{
	
	if (local)
	{
		local_pos_vector += new_pos;
		XMStoreFloat3(&local_pos, local_pos_vector);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		pos_vector += new_pos;
		XMStoreFloat3(&pos, pos_vector);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::AdjustPosition(const XMFLOAT3& new_pos, bool local, bool orbital)
{
	
	if (local)
	{
		local_pos.x += new_pos.x;
		local_pos.y += new_pos.y;
		local_pos.z += new_pos.z;
		local_pos_vector = XMLoadFloat3(&local_pos);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		pos.x += new_pos.x;
		pos.y += new_pos.y;
		pos.z += new_pos.z;
		pos_vector = XMLoadFloat3(&pos);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::AdjustPosition(float x, float y, float z, bool local, bool orbital)
{
	
	if (local)
	{
		local_pos.x += x;
		local_pos.y += y;
		local_pos.z += z;
		local_pos_vector = XMLoadFloat3(&local_pos);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		pos.x += x;
		pos.y += y;
		pos.z += z;
		pos_vector = XMLoadFloat3(&pos);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::SetRotation(const XMVECTOR& new_rot, bool local, bool orbital)
{
	
	if (local)
	{
		local_rot_vector = new_rot;
		XMStoreFloat3(&local_rot, new_rot);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		rot_vector = new_rot;
		XMStoreFloat3(&rot, new_rot);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::SetRotation(const XMFLOAT3& new_rot, bool local , bool orbital )
{
	
	if (local)
	{
		local_rot = new_rot;
		local_rot_vector = XMLoadFloat3(&local_rot);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		rot = new_rot;
		rot_vector = XMLoadFloat3(&rot);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::SetRotation(float x, float y, float z, bool local , bool orbital )
{
	
	if (local)
	{
		local_rot = XMFLOAT3(x, y, z);
		local_rot_vector = XMLoadFloat3(&local_rot);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		rot = XMFLOAT3(x, y, z);
		rot_vector = XMLoadFloat3(&rot);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::AdjustRotation(const XMVECTOR& new_rot, bool local , bool orbital )
{
	
	if (local)
	{
		local_rot_vector += new_rot;
		XMStoreFloat3(&local_rot, local_rot_vector);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		rot_vector += new_rot;
		XMStoreFloat3(&rot, rot_vector);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::AdjustRotation(const XMFLOAT3& new_rot, bool local, bool orbital)
{
	
	if (local)
	{
		local_rot.x += new_rot.x;
		local_rot.y += new_rot.y;
		local_rot.z += new_rot.z;
		local_rot_vector = XMLoadFloat3(&local_rot);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		rot.x += new_rot.x;
		rot.y += new_rot.y;
		rot.z += new_rot.z;
		rot_vector = XMLoadFloat3(&rot);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::AdjustRotation(float x, float y, float z, bool local, bool orbital)
{
	
	if (local)
	{
		local_rot.x += x;
		local_rot.y += y;
		local_rot.z += z;
		local_rot_vector = XMLoadFloat3(&local_rot);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		rot.x += x;
		rot.y += y;
		rot.z += z;
		rot_vector = XMLoadFloat3(&rot);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::SetScale(float x, float y, float z, bool local, bool orbital)
{
	if (local)
	{
		local_scale = XMFLOAT3(x, y, z);
		local_scale_vector = XMLoadFloat3(&scale);
		UpdateLocalMatrix(orbital);
	}
	else
	{
		scale = XMFLOAT3(x, y, z);
		scale_vector = XMLoadFloat3(&scale);
	}
	if (local && orbital)
	{
		orbital = false;
	}
	UpdateWorldMatrix(orbital);
}

void Model::RotateByRadiusAroundY(float theta_delta, float radius, bool local, float adjust_x, float adjust_y, float adjust_z)
{
	theta += theta_delta;
	SetPosition(adjust_x + radius * cos(theta), adjust_z + adjust_y, radius * sin(theta), local, false);
}

void Model::SetLookAtPos(XMFLOAT3 look_at_pos)
{
	// Verify that at_pos is not the same as cam pos.
	if (look_at_pos.x == pos.x &&
		look_at_pos.y == pos.y &&
		look_at_pos.z == pos.z)
	{
		return;
	}

	look_at_pos.x = pos.x - look_at_pos.x;
	look_at_pos.y = pos.y - look_at_pos.y;
	look_at_pos.z = pos.z - look_at_pos.z;

	float pitch = 0.0f;
	if (look_at_pos.y != 0.0f)
	{
		const float distance = sqrt(look_at_pos.x * look_at_pos.x + look_at_pos.z * look_at_pos.z);
		pitch = atan(look_at_pos.y / distance);
	}

	float yaw = 0.0f;
	if (look_at_pos.x != 0.0f)
	{
		yaw = atan(look_at_pos.x / look_at_pos.z);
	}
	if (look_at_pos.z > 0)
	{
		yaw += XM_PI;
	}

	SetRotation(pitch, yaw, 0.0f, false);
}

//void Model::RotateByOrbit(float pos_x, float pos_y, float pos_z, float rot_x, float rot_y, float rot_z)
//{
//	local_matrix = XMMatrixTranslation(pos.x, pos.y, pos.z) * XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * local_matrix;
//
//	world_matrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * local_matrix;
//
//	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, rot.y, 0.0f);
//	vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
//	vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
//	vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
//	vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
//}

const XMVECTOR& Model::GetForwardVector(bool omit_Y)
{
	if (omit_Y)
	{
		return vec_forward_no_Y;
	}
	else
	{
		return vec_forward;
	}
}

const XMVECTOR& Model::GetRightVector(bool omit_Y)
{
	if (omit_Y)
	{
		return vec_right_no_Y;
	}
	else
	{
		return vec_right;
	}
}

const XMVECTOR& Model::GetBackwardVector(bool omit_Y)
{
	if (omit_Y)
	{
		return vec_backward_no_Y;
	}
	else
	{
		return vec_backward;
	}
}

const XMVECTOR& Model::GetLeftVector(bool omit_Y)
{
	if (omit_Y)
	{
		return vec_left_no_Y;
	}
	else
	{
		return vec_left;
	}
}