#include <motor/other/ModelLoader.h>

using namespace DirectX;

// public
bool ModelLoader::Initialize(ID3D11Device* device) 
{
    _device = device;
    return true;
}

bool ModelLoader::LoadModel(const std::string& filePath) 
{
    _directory = StringHelper::GetDirectoryFromPath(filePath);

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filePath,
      aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr)
        return false;

    // Bounding box min max vertex params
    XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
    XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

    vMin = XMLoadFloat3(&vMinf3);
    vMax = XMLoadFloat3(&vMaxf3);

    ProcessNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());

    XMStoreFloat3(&_model.bounding_box.Center, 0.5f * (vMin + vMax));
    XMStoreFloat3(&_model.bounding_box.Extents, 0.5f * (vMax - vMin));

    // Add struct to struct array (mesh and bounding box)
    _models.push_back(_model);

    return true;
}

ModelStruct& ModelLoader::GetModelById(int id) 
{
    return _models[id];
}

int ModelLoader::CountOfModelsLoaded()
{
    return _models.size();
}


// private
void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix) 
{
    XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

    for (UINT i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _model.meshes.push_back(ProcessMesh(mesh, scene, nodeTransformMatrix));
    }

    for (UINT i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, nodeTransformMatrix);
    }
}

MeshForComponents ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix) {
    // Data to fill
    std::vector<Vertex> vertices;
    std::vector<DWORD> indices;

    //Get vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.pos.x = mesh->mVertices[i].x;
        vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;

        XMVECTOR P = XMLoadFloat3(&vertex.pos);

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);

        vMin = XMVectorMin(vMin, P);
        vMax = XMVectorMax(vMax, P);
    }

    //Get indices
    for (UINT i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    std::vector<Texture> textures;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
    textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

    return MeshForComponents(_device, vertices, indices, textures, transformMatrix);
}

TextureStorageType ModelLoader::DetermineTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType textureType) {
    if (pMat->GetTextureCount(textureType) == 0)
        return TextureStorageType::None;

    aiString path;
    pMat->GetTexture(textureType, index, &path);
    std::string texturePath = path.C_Str();
    //Check if texture is an embedded indexed texture by seeing if the file path is an index #
    if (texturePath[0] == '*') {
        if (pScene->mTextures[0]->mHeight == 0) {
            return TextureStorageType::EmbeddedIndexCompressed;
        } else {
            assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
            return TextureStorageType::EmbeddedIndexNonCompressed;
        }
    }
    //Check if texture is an embedded texture but not indexed (path will be the texture's name instead of #)
    if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str())) {
        if (pTex->mHeight == 0) {
            return TextureStorageType::EmbeddedCompressed;
        } else {
            assert("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
            return TextureStorageType::EmbeddedNonCompressed;
        }
    }
    //Lastly check if texture is a filepath by checking for period before extension name
    if (texturePath.find('.') != std::string::npos) {
        return TextureStorageType::Disk;
    }

    return TextureStorageType::None; // No texture exists
}

std::vector<Texture> ModelLoader::LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene) {
    std::vector<Texture> materialTextures;
    TextureStorageType storetype = TextureStorageType::Invalid;
    unsigned int textureCount = pMaterial->GetTextureCount(textureType);

    if (textureCount == 0) //If there are no textures
    {
        storetype = TextureStorageType::None;
        aiColor3D aiColor(0.0f, 0.0f, 0.0f);
        switch (textureType) {
        case aiTextureType_DIFFUSE:
            pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
            if (aiColor.IsBlack()) //If color = black, just use grey
            {
                materialTextures.push_back(Texture(_device, Colors::UnloadedTextureColor, textureType));
                return materialTextures;
            }
            materialTextures.push_back(Texture(_device, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), textureType));
            return materialTextures;
        }
    } else {
        for (UINT i = 0; i < textureCount; i++) {
            aiString path;
            pMaterial->GetTexture(textureType, i, &path);
            TextureStorageType storetype = DetermineTextureStorageType(pScene, pMaterial, i, textureType);
            switch (storetype) {
            case TextureStorageType::EmbeddedIndexCompressed: {
                int index = GetTextureIndex(&path);
                Texture embeddedIndexedTexture(_device,
                  reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData),
                  pScene->mTextures[index]->mWidth,
                  textureType);
                materialTextures.push_back(embeddedIndexedTexture);
                break;
            }
            case TextureStorageType::EmbeddedCompressed: {
                const aiTexture* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
                Texture embeddedTexture(_device,
                  reinterpret_cast<uint8_t*>(pTexture->pcData),
                  pTexture->mWidth,
                  textureType);
                materialTextures.push_back(embeddedTexture);
                break;
            }
            case TextureStorageType::Disk: {
                std::string filename = _directory + '\\' + path.C_Str();
                Texture diskTexture(_device, filename, textureType);
                materialTextures.push_back(diskTexture);
                break;
            }
            }
        }
    }

    if (materialTextures.size() == 0) {
        materialTextures.push_back(Texture(_device, Colors::UnhandledTextureColor, aiTextureType::aiTextureType_DIFFUSE));
    }
    return materialTextures;
}

int ModelLoader::GetTextureIndex(aiString* pStr) {
    assert(pStr->length >= 2);
    return atoi(&pStr->C_Str()[1]);
}