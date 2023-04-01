#pragma once
#include "Gameobject.h"
#include "Shaders.h"
#include "SkinnedMesh.h"
#include "ResourceManager.h"

/// <summary>
/// �ִϸ��̼��̵���ִ� ���� 3D���� �ε��Ҷ� ����ϴ� ������Ʈ �Դϴ�. 
/// </summary>
class SkinnedMeshRenderer : public Component
{
public:

	SkinnedMeshRenderer(GameObject* owner) : Component(owner)
	{
		this->type = Component::Type::RENDERER_SKINNED_MODEL;
		this->name = "SkinnedMeshRenderer";
	}

	bool SetSkinnedMesh(SkinnedMesh* model);

	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;

	std::string GetPath();
	const aiScene* GetAiScene();

	bool isStart = false;
private:
	SkinnedMesh* model;
	std::vector<XMMATRIX> matrix;
	long long StartTimeMillis = 0;
};