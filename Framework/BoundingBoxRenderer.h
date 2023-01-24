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
			Logger::Log(L"�ش� ������Ʈ�� ���� ���������ʽ��ϴ�. ��輱�� �׷����� �ʽ��ϴ�.");
		}
	}

	//void processNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix)
	//{
	//	//�θ� ��� ���� ��ǥ��� ��ȯ
	//	XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

	//	DirectX::XMFLOAT4 v;
	//	DirectX::XMStoreFloat4(&v, nodeTransformMatrix.r[3]);
	//	BBox(v);
	//	


	//	////�ش� ����� �޽ø� ��ȸ 
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
		//�θ� ��� ���� ��ǥ��� ��ȯ
		XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;


		//�ش� ����� �޽ø� ��ȸ 
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
	//���ؽ� ����
	VertexBuffer<XMFLOAT3> vertexbuffer;
	//�ε��� ����
	IndexBuffer indexbuffer;
	//���ؽ�Ʈ
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

};