#pragma once
#include "Gameobject.h"
#include "Renderer.h"

class MeshRenderer : public Component , public Renderer
{
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;
	virtual std::string GetPath() override;
};