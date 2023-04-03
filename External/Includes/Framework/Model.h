#pragma once
#include "Mesh.h"
#include "Matrix.h"
#include "Texture.h"
#include "Shaders.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <dtex\DirectXTex.h>

using namespace DirectX;

class ResourceManager;
/*
    Assimp 예제 , LearnOpenGL Model 참고
*/
class Model
{
public:
    Model() {}

    Model(const Model& rhs) : meshes(rhs.meshes)
    {
        this->modelName = rhs.modelName;
        this->directory = rhs.directory;
        this->filePath = rhs.filePath;
        this->pScene = rhs.pScene;
        this->textures_loaded_ = rhs.textures_loaded_;
        this->device = rhs.device;
        this->deviceContext = rhs.deviceContext;
        this->constantBuffer = rhs.constantBuffer;
        this->vertexShader = rhs.vertexShader;
        this->pixelShader = rhs.pixelShader;
    }

	bool Init(const std::string& modelName,const std::string& filePath, ResourceManager* res);
	void Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix);

    std::vector<Mesh>& GetMeshes();
    const aiScene* getAiScene();
    std::string GetPath();
    std::string GetName();

private:
    /*  Model 데이터  */

    std::vector<Mesh> meshes;
    std::string modelName;
    std::string directory;
    std::string filePath;

    Assimp::Importer Importer;
    const aiScene* pScene = nullptr;
    /*  함수   */
    bool loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, const XMMATRIX& transformMatrix);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName, const aiScene* scene);
    ID3D11ShaderResourceView* loadEmbeddedTexture(const aiTexture* embeddedTexture);

    std::vector<Texture>* textures_loaded_;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	ConstantBuffer<CB_VS_2>* constantBuffer = nullptr;

    VertexShader* vertexShader;
    PixelShader* pixelShader;

};

