#pragma once
#include "pch.h"
#include "Component.h"
#include "Renderer.h"
#include "Shaders.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "ConstantBufferType.h"

#include <directxcollision.h>

class GameObject;
class ResourceManager;

class BoundingBoxRenderer : public Component
{
public:
	BoundingBoxRenderer(GameObject* owner) : Component(owner) {}

	virtual void Draw(const XMMATRIX& viewProjectionMatrix) {};

	void SetBoundingBoxActive(bool value)
	{
		this->isActiveBoundingBox = value;
	}

	bool isActive()
	{
		return isActiveBoundingBox;
	}


protected:
	bool isActiveBoundingBox = true;
};

class BoundingBox3D : public BoundingBoxRenderer
{
public:

	BoundingBox3D(GameObject* owner, ResourceManager* res);

	//���ڽ� ��� �ڵ�
	void processNode(aiNode* node, const aiScene* scene, const XMMATRIX& parentTransformMatrix)
	{
		//�θ� ��� ���� ��ǥ��� ��ȯ
		XMMATRIX nodeTransformMatrix = XMMatrixTranspose(XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

		//�ش� ����� �޽ø� ��ȸ 
		for (UINT i = 0; i < node->mNumMeshes; i++) {

			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
				DirectX::XMFLOAT4 v(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 0.0f);
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

	//������ �׸��ϴ�.
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;


	DirectX::XMFLOAT3 min = { -3,-3,-3 };
	DirectX::XMFLOAT3 max = { 3,3,3 };

	bool isActive = true;

	std::vector<SimpleVertex> vertices;

	DWORD tri[36] =
	{
	   3,1,0,
	   2,1,3,

	   0,5,4,
	   1,5,0,

	   3,4,7,
	   0,4,3,

	   1,6,5,
	   2,6,1,

	   2,7,6,
	   3,7,2,

	   6,4,5,
	   7,4,6
	};

	//���̴�
	VertexShader* vs;
	PixelShader* ps;

	//�������
	ConstantBuffer<CB_VS_1>* constantBuffer = nullptr;
	//���ؽ� ����
	VertexBuffer<SimpleVertex> vertexbuffer;
	//�ε��� ����
	IndexBuffer indexbuffer;

	//���ؽ�Ʈ - �ܺο��� ����� �ɴϴ�. 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;

	

};

class BoundingBox2D : public BoundingBoxRenderer
{
public:

};