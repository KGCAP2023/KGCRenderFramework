#pragma once
#include "pch.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ModelRenderer.h"
#include "Matrix.h"
#include "Shaders.h"
#include <directxcollision.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class BoundingBoxRenderer : public Component, public Renderer
{
public:
	BoundingBoxRenderer(GameObject* owner, ID3D11Device* device, ID3D11DeviceContext* deviceContext, PixelShader* ps) : Component(owner)
	{
		this->type = Component::Type::BOUNDING_BOX;
		this->deviceContext = deviceContext;
		this->ps = ps;
		
		Renderer* model = dynamic_cast<Renderer*>(owner->GetComponent(Component::Type::RENDERER_MODEL));
		if (model != nullptr)
		{
			XMMATRIX worldMatrix = owner->transform.worldMatrix;
			//std::vector<Mesh>& meshes = model->GetMeshes();
			
			Assimp::Importer importer;

			const aiScene* pScene = importer.ReadFile(model->GetPath(),
				aiProcess_Triangulate |
				aiProcess_ConvertToLeftHanded);

			processNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());

			std::vector<XMFLOAT3> vertices;
			vertices.push_back(XMFLOAT3(min.x, max.y, min.z));
			vertices.push_back(XMFLOAT3(max.x, max.y, min.z));
			vertices.push_back(XMFLOAT3(max.x, max.y, max.z));
			vertices.push_back(XMFLOAT3(min.x, max.y, max.z));
			vertices.push_back(XMFLOAT3(min.x, min.y, min.z));
			vertices.push_back(XMFLOAT3(max.x, min.y, min.z));
			vertices.push_back(XMFLOAT3(max.x, min.y, max.z));
			vertices.push_back(XMFLOAT3(min.x, min.y, max.z));

			//DWORD arr[] =
			//{
			//	3,1,0,
			//	2,1,3,

			//	0,5,4,
			//	1,5,0,

			//	3,4,7,
			//	0,4,3,

			//	1,6,5,
			//	2,6,1,

			//	2,7,6,
			//	3,7,2,

			//	6,4,5,
			//	7,4,6,
			//};

			DWORD arr[] =
			{
				0, 1,
				1, 2,
				2, 3,
				3, 0,
				4, 5,
				5, 6,
				6, 7,
				7, 4,
				0, 4,
				1, 5,
				2, 6,
				3, 7
			};


			std::vector<DWORD> indices(std::begin(arr), std::end(arr));

			HRESULT hr = this->vertexbuffer.Init(vertices.data(), vertices.size(), device);
			hr = this->indexbuffer.Init(indices.data(), indices.size(), device);
		}
		else
		{
			Logger::Log(L"해당 오브젝트에 모델이 존재하지않습니다. 경계선이 그려지지 않습니다.");
		}
	}

	//void processNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix)
	//{
	//	//부모 노드 기준 좌표계로 변환
	//	XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

	//	DirectX::XMFLOAT4 v;
	//	DirectX::XMStoreFloat4(&v, nodeTransformMatrix.r[3]);
	//	BBox(v);
	//	


	//	////해당 노드의 메시를 순회 
	//	//for (UINT i = 0; i < node->mNumMeshes; i++) {

	//	//	aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	//	//	std::cout << node->mName.C_Str() << std::endl;

	//	//	Matrix::print(nodeTransformMatrix);
	//	//	meshes.push_back(this->processMesh(mesh, scene, nodeTransformMatrix));
	//	//}

	//	for (UINT i = 0; i < node->mNumChildren; i++) {
	//		this->processNode(node->mChildren[i], scene, nodeTransformMatrix);
	//	}
	//}

	void processNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix)
	{
		//부모 노드 기준 좌표계로 변환
		XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;


		//해당 노드의 메시를 순회 
		for (UINT i = 0; i < node->mNumMeshes; i++) {

			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				DirectX::XMFLOAT4 v(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,0.0f);
				BBox(v);
			}
		}

		for (UINT i = 0; i < node->mNumChildren; i++) {
			this->processNode(node->mChildren[i], scene, nodeTransformMatrix);
		}
	}

	void BBox(DirectX::XMFLOAT4& v)
	{
		float x = v.x;
		float y = v.y;
		float z = v.z;

		if (x < min.x) min.x = x;
		if (y < min.y) min.y = y;
		if (z < min.z) min.z = z;
		if (x > max.x) max.x = x;
		if (y > max.y) max.y = y;
		if (z > max.z) max.z = z;
	}

	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;
	
	DirectX::XMFLOAT3 min = {0,0,0};
	DirectX::XMFLOAT3 max = {0,0,0};

	bool isActive = true;

	PixelShader* ps;
	//버텍스 버퍼
	VertexBuffer<XMFLOAT3> vertexbuffer;
	//인덱스 버퍼
	IndexBuffer indexbuffer;
	//컨텍스트
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

};