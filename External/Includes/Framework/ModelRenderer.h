#pragma once
#include "Gameobject.h"
#include "Shaders.h"
#include "Renderer.h"
#include "Model.h"

/// <summary>
/// ���� ���� �ҷ��ö� �ַ� ����ϴ� ������Ʈ �Դϴ�.
/// </summary>
class ModelRenderer : public Component, public Renderer
{
public:
	ModelRenderer(GameObject* owner, ResourceManager* res) : Component(owner)
	{
		this->type = Component::Type::RENDERER_MODEL;
		this->name = "ModelRenderer";
		this->res = res;
	}

	bool SetModel(Model* model);
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;

	virtual void InitBoundingBox() override;
	virtual BoundingBoxRenderer* GetBoundingBox() override;

	std::vector<Mesh>& GetMeshes();
	std::string GetPath();
private:
	Model* model = nullptr;
	BoundingBoxRenderer* bbox = nullptr;
	ResourceManager* res = nullptr;
};