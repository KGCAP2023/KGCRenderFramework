#pragma once
#include "..\pch.h"
#include "Gameobject.h"
#include "Renderer.h"

class ModelRenderer : public Component, public Renderer
{
public:
	ModelRenderer(GameObject* owner) : Component(owner) 
	{
		this->type = Component::Type::RENDERER_MODEL;
	}

	bool Init(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_2>& cb_vs_vertexshader);
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;

private:
	Model model;
	
};