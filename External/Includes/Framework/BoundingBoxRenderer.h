#pragma once
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
	BoundingBoxRenderer(GameObject* owner, ID3D11Device* device, ID3D11DeviceContext* deviceContext, PixelShader* ps, VertexShader* vs, ConstantBuffer<CB_VS_1>& constantBuffer) : Component(owner)
	{
		this->type = Component::Type::BOUNDING_BOX;
		this->deviceContext = deviceContext;
		this->ps = ps;
		this->vs = vs;
		this->constantBuffer = &constantBuffer;


		//Renderer를 상속받은 클래스를 Renderer 클래스로 다운캐스팅 합니다. 
		Renderer* model = dynamic_cast<Renderer*>(owner->GetComponent(Component::Type::RENDERER_MODEL));
		if (model != nullptr)
		{
			min = { 0,0,0 };
			max = { 0,0,0 };

			XMMATRIX worldMatrix = owner->transform.worldMatrix;
			
			Assimp::Importer importer;
			const aiScene* pScene = importer.ReadFile(model->GetPath(),
				aiProcess_Triangulate |
				aiProcess_ConvertToLeftHanded);

			processNode(pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());

		}

		//버텍스를 정의합니다.
		//<주의> 
		// 버텍스 쉐이더에 들어가는 구조체(Vertex.h)와 동일해야합니다

		std::vector<SimpleVertex> vertices;
		vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, max.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, min.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices.push_back(SimpleVertex{ XMFLOAT3(max.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });
		vertices.push_back(SimpleVertex{ XMFLOAT3(min.x, min.y, max.z), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) });

		//정점에대한 인덱스를 정의합니다. 
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

		//버텍스 버퍼와 인덱스 버퍼에 등록합니다.
		HRESULT hr = this->vertexbuffer.Init(vertices.data(), vertices.size(), device);
		hr = this->indexbuffer.Init(indices.data(), indices.size(), device);

	}

	//경계박스 계산 코드
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

	//도형을 그립니다.
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;

	//여기서는 안씀
	virtual void Update() override;
	
	DirectX::XMFLOAT3 min = {-3,-3,-3};
	DirectX::XMFLOAT3 max = {3,3,3};

	bool isActive = true;

	//쉐이더
	VertexShader* vs;
	PixelShader* ps;

	//상수버퍼
	ConstantBuffer<CB_VS_1>* constantBuffer = nullptr;
	//버텍스 버퍼
	VertexBuffer<SimpleVertex> vertexbuffer;
	//인덱스 버퍼
	IndexBuffer indexbuffer;


	//컨텍스트 - 외부에서 끌어다 옵니다. 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

};