#include "pch.h"
#include "Texture.h"

Texture::Texture(ID3D11Device* device, const std::string& filePath)
{

}

Texture::Texture()
{
}

Texture::~Texture()
{
}

ID3D11ShaderResourceView** Texture::GetTextureResourceViewAddress()
{
    return this->texture.GetAddressOf();
}

ID3D11ShaderResourceView* Texture::Get()
{
	return this->texture.Get();
}

void Texture::Initialize1x1ColorTexture(ID3D11Device* device, const RGBColor& colorData)
{
	InitializeColorTexture(device, &colorData, 1, 1);
}

void Texture::InitializeColorTexture(ID3D11Device* device, const RGBColor* colorData, UINT width, UINT height)
{
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* p2DTexture = nullptr;
	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = colorData;
	initialData.SysMemPitch = width * sizeof(RGBColor);
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);

	texture2d = static_cast<ID3D11Texture2D*>(p2DTexture);
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	hr = device->CreateShaderResourceView(texture2d.Get(), &srvDesc, texture.GetAddressOf());
}