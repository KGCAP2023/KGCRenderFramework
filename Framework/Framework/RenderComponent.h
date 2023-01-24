#pragma once
#include "..\pch.h"
#include "Gameobject.h"
#include "Renderer.h"

class RenderComponent : public Component, public Renderer
{
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;

	std::vector<Component> renderComponents;
};

