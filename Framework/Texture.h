#pragma once
#include "pch.h"

class Texture
{
public:
	Texture(ID3D11Device* device, const std::string& filePath);
	Texture();
	~Texture();
	Texture(const Texture& rhs)
	{
		type = rhs.type;
		path = rhs.path;
		texture = rhs.texture.Get();
	}

	ID3D11ShaderResourceView** GetTextureResourceViewAddress();

	//�ε� �ؽ���
	std::string type;
	std::string path;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr;
};

