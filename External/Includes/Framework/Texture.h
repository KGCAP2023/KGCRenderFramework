#pragma once
#include "pch.h"
#include "Color.h"

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

	void Initialize1x1ColorTexture(ID3D11Device* device, const RGBColor& colorData);
	void InitializeColorTexture(ID3D11Device* device, const RGBColor* colorData, UINT width, UINT height);

	ID3D11ShaderResourceView** GetTextureResourceViewAddress();
	ID3D11ShaderResourceView* Get();
	//로드 텍스쳐
	std::string type;
	std::string path;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr;
};

