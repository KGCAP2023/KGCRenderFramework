#pragma once
#include "..\pch.h"
#include "GameObject3D.h"
#include "SimpleMesh.h"

class SimpleRenderableObject : public GameObject3D
{
public:
	SimpleRenderableObject() {}
	bool Init(ID3D11Device* device,
		ID3D11DeviceContext* deviceContext,
		ConstantBuffer<CB_VS_1>& constantBuffer);
	void Draw(const XMMATRIX& viewProjectionMatrix) override;

protected:
	
	SimpleMesh mesh;
	ConstantBuffer<CB_VS_1>* constantBuffer;
	ID3D11DeviceContext* deviceContext;

	virtual void Update() override;

	XMMATRIX worldMatrix = XMMatrixIdentity();

};
