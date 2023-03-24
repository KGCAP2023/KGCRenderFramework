#pragma once
#include "Gameobject.h"


class MeshRenderer : public Component
{
	virtual void Draw(const XMMATRIX& viewProjectionMatrix) override;
	virtual void Update() override;
};