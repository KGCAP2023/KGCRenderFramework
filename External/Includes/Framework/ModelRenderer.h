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
	ModelRenderer(GameObject* owner) : Component(owner) 
	{
		this->type = Component::Type::RENDERER_MODEL;
		this->deleteType = Component::Type::RENDERER_MODEL;
		this->name = "ModelRenderer";
	}

	bool SetModel(Model* model);
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;
	std::vector<Mesh>& GetMeshes();
	std::string GetPath();
private:
	Model* model;
	
};