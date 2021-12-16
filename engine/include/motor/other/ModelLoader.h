#pragma once

#include <motor/graphics/MeshForComponents.h>
#include <motor/other/MathHelper.h>

#include <DirectXCollision.h>

struct ModelStruct 
{
    ModelStruct() = default;

    std::vector<MeshForComponents> meshes;
    DirectX::BoundingOrientedBox bounding_box;
    std::string model_name;
};

class ModelLoader {
public:
    bool Initialize(ID3D11Device* device);
    bool LoadModel(const std::string& filePath, std::string name = "No name");
    ModelStruct& GetModelById(int id);


private:
    std::vector<ModelStruct> _models;

    void ProcessNode(aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix, ModelStruct& temp_model);
    MeshForComponents ProcessMesh(aiMesh* mesh, const aiScene* scene, const DirectX::XMMATRIX& transformMatrix);
    TextureStorageType DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);
    int GetTextureIndex(aiString* pStr);

    ID3D11Device* _device = nullptr;

    DirectX::XMVECTOR vMin;
    DirectX::XMVECTOR vMax;

    std::string _directory = "";

    friend class SceneHierarchy;
 };