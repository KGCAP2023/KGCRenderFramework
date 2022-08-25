#pragma once
#include "..\pch.h"

class Renderer
{
public:
	virtual void Draw(const DirectX::XMMATRIX& viewProjectionMatrix);
};

