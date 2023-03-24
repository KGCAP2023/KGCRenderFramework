#pragma once
#include "Gameobject.h"
#include "Shaders.h"
#include "Renderer.h"
#include "Model.h"

class ModelRenderer : public Component, public Renderer
{
public:
	ModelRenderer(GameObject* owner) : Component(owner) 
	{
		this->type = Component::Type::RENDERER_MODEL;
	}

	bool Init(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_2>& cb_vs_vertexshader, VertexShader* vertexShader,PixelShader* pixelShader);
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;
	std::vector<Mesh>& GetMeshes();
	std::string GetPath();
private:
	Model model;
	
};