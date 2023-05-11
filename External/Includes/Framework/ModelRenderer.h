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
	ModelRenderer(GameObject* owner, ResourceManager* res) : Component(owner)
	{
		this->type = Component::Type::RENDERER_MODEL;
		this->name = "ModelRenderer";
		this->res = res;
	}

	~ModelRenderer()
	{
		std::cout << "[=] ModelRenderer destructor called" << std::endl;
		this->model = nullptr;
		this->bbox = nullptr;
		this->res = nullptr;
	}

	ModelRenderer(const ModelRenderer& rhs, GameObject* owner) : Component(owner)
	{
		std::cout << "[=] ModelRenderer CLONE Process - Copy constructor called" << std::endl;
		this->type = rhs.type;
		this->name = rhs.name;
		this->res = rhs.res;
		this->model = rhs.model;
		this->bbox = rhs.bbox;
	}

	virtual Component* Copy(GameObject* owner)
	{
		Component* compo = new ModelRenderer(*this,owner);
		return compo;
	};

	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;

	virtual void InitBoundingBox() override;
	virtual BoundingBoxRenderer* GetBoundingBox() override;
	virtual std::string GetName() override;

	bool SetModel(Model* model);
	std::vector<Mesh>& GetMeshes();
	std::string GetPath();
private:
	Model* model = nullptr;
	BoundingBoxRenderer* bbox = nullptr;
	ResourceManager* res = nullptr;
};