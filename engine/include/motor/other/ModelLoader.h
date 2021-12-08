#pragma once

#include <motor/graphics/MeshForComponents.h>
#include <motor/other/MathHelper.h>

#include <DirectXCollision.h>

struct ModelStruct 
{
    std::vector<MeshForComponents> meshes;
    DirectX::BoundingOrientedBox bounding_box;
};

class ModelLoader {
public:
    bool Initialize(ID3D11Device* device);
    bool LoadModel(const std::string& filePath);
    ModelStruct& GetModelById(int id);
    int CountOfModelsLoaded();

private:
    std::vector<ModelStruct> _models;
    ModelStruct _model;

    void ProcessNode(aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix);
    MeshForComponents ProcessMesh(aiMesh* mesh, const aiScene* scene, const DirectX::XMMATRIX& transformMatrix);
    TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
    int GetTextureIndex(aiString* pStr);

    ID3D11Device* _device = nullptr;

    DirectX::XMVECTOR vMin;
    DirectX::XMVECTOR vMax;

    std::string _directory = "";
};