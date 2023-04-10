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
#include "Texture.h"
#include <SpriteBatch.h>
#include <directxcollision.h>

class GameObject;
class ResourceManager;

class BoundingBoxRenderer : public Component
{
public:
	static enum class ShapeType { CUBE, HOUR_GLASS };

	BoundingBoxRenderer(GameObject* owner) : Component(owner) {}
	virtual void Update() {};
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) {};
	virtual void ChangeDrawShape(ShapeType _type) {};
	virtual std::vector<DWORD>* GetIndices() { return nullptr; }
	virtual void ChangeColor(float r, float g, float b, float alpha = 1.0f) {}

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
	virtual void Update() override;

	BoundingBox3D(GameObject* owner, Component* component, ResourceManager* res);

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
	virtual std::vector<DWORD>* GetIndices() override;
	virtual void ChangeColor(float r, float g, float b, float alpha = 1.0f) override;

	DirectX::XMFLOAT3 min = { -1,-1,-1 };
	DirectX::XMFLOAT3 max = { 1,1,1 };

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
	Microsoft::WRL::ComPtr<ID3D11Device> device;

	/// <summary>
	/// �׸��� ���� ����, ���� ������ ����
	/// </summary>
	void DrawSetting();

	/// <summary>
	/// �׷����� ��� ����
	/// </summary>
	/// <param name="_type">enum �����Ͽ� ����</param>
	/// <param name="_g">��� ���ӿ�����Ʈ</param>
	virtual void ChangeDrawShape(ShapeType _type) override;

private:

	std::vector<DWORD> indices;

};

class BoundingBox2D : public BoundingBoxRenderer
{
public:
	BoundingBox2D(GameObject* owner, Component* component, ResourceManager* res);

	//������ �׸��ϴ�.
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual std::vector<DWORD>* GetIndices() override;
	virtual void ChangeColor(float r, float g, float b, float alpha = 1.0f) override;

	XMFLOAT2 CalculateRotation(LONG x, LONG y, XMMATRIX& rotationMatrix);

	std::vector<SimpleVertex> vertices;

	Texture* color;

	float width = 0;
	float height = 0;
	int lineWidth = 1;

	SpriteBatch* spriteBatch;

	//���ؽ�Ʈ - �ܺο��� ����� �ɴϴ�. 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	std::vector<DWORD> indices;
};