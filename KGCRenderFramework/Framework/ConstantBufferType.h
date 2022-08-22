#pragma once
#include "..\pch.h"

struct CB_VS_1
{
    DirectX::XMMATRIX WVP;
};

struct CB_VS_2
{
    DirectX::XMMATRIX WVP;
    DirectX::XMMATRIX W;
};

struct CB_PS_light
{
	DirectX::XMFLOAT3 ambientLightColor;
	float ambientLightStrength;

	DirectX::XMFLOAT3 dynamicLightColor;
	float dynamicLightStrength;
	DirectX::XMFLOAT3 dynamicLightPosition;
	float dynamicLightAttenuation_a;
	float dynamicLightAttenuation_b;
	float dynamicLightAttenuation_c;
};