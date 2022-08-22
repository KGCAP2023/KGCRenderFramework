#pragma once
#include "GameObject3D.h"
#include "Model.h"

class RenderableObject : public GameObject3D
{

public:
	bool Init(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_2>& cb_vs_vertexshader);
	void Draw(const XMMATRIX& viewProjectionMatrix) override;

protected:
	
	virtual void Update() override;

	Model model;
	XMMATRIX worldMatrix = XMMatrixIdentity();
};

