#pragma once
#include "Gameobject.h"
#include "Shaders.h"
#include "Renderer.h"
#include "Model.h"

/// <summary>
/// 정적 모델을 불러올때 주로 사용하는 컴포넌트 입니다.
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