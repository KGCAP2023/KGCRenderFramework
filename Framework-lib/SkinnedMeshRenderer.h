#pragma once
#include "Gameobject.h"
#include "Shaders.h"
#include "SkinnedMesh.h"

/// <summary>
/// �ִϸ��̼��̵���ִ� ���� 3D���� �ε��Ҷ� ����ϴ� ������Ʈ �Դϴ�. 
/// </summary>
class SkinnedMeshRenderer : public Component
{
public:

	SkinnedMeshRenderer(GameObject* owner) : Component(owner)
	{
		this->type = Component::Type::RENDERER_SKINNED_MODEL;
	}

	bool Init(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_2>& cb_vs_vertexshader, ConstantBuffer<CB_VS_3>& cb_vs_vertexshader2 ,VertexShader* vertexShader,
		PixelShader* pixelShader);

	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;

	std::string GetPath();
	const aiScene* GetAiScene();

	bool isStart = false;
private:
	SkinnedMesh model;
	std::vector<XMMATRIX> matrix;
	long long StartTimeMillis = 0;
};