#pragma once
#include "..\pch.h"
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

/*
    Assimp 예제 , LearnOpenGL Model 참고
*/
class Model
{
public:

	bool Init(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_2>& cb_vs_vertexshader, VertexShader* vertexShader, PixelShader* pixelShader);
	void Draw(const XMMATRIX& worldMatrix, const XMMATRIX& viewProjectionMatrix);

    std::vector<Mesh>& GetMeshes();
    const aiScene* getAiScene();
    std::string GetPath();

private:
    /*  Model 데이터  */

    std::vector<Mesh> meshes;
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

    std::vector<Texture> textures_loaded_;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

	ConstantBuffer<CB_VS_2>* constantBuffer = nullptr;

    VertexShader* vertexShader;
    PixelShader* pixelShader;

};

