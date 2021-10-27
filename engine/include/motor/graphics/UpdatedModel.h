#pragma once

#include <motor/graphics/Mesh.h>
#include <motor/other/MathHelper.h>

#include <DirectXCollision.h>

class UpdatedModel
{
public:
	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertex_shader>& cb_vs_vertexshader);
	//void SetTexture(ID3D11ShaderResourceView* texture);
	void Draw(const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewProjectionMatrix);
	DirectX::BoundingBox& GetBoundingBox();


private:
	std::vector<Mesh> meshes;
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix, DirectX::XMVECTOR vMin, DirectX::XMVECTOR vMax);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const DirectX::XMMATRIX& transformMatrix, DirectX::XMVECTOR vMin, DirectX::XMVECTOR vMax);
	TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
	int GetTextureIndex(aiString* pStr);

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ConstantBuffer<CB_VS_vertex_shader>* cb_vs_vertexshader = nullptr;
	DirectX::BoundingBox bounding_box;
	
	//ID3D11ShaderResourceView* texture = nullptr;
	std::string directory = "";
};
