#include "pch.h"
#include "Sprite.h"

bool Sprite::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string spritePath)
{
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;

	HRESULT hr = DirectX::CreateWICTextureFromFile(device, StringUtil::StringToWide(spritePath).c_str(),
			resource.GetAddressOf(),
			this->view.GetAddressOf());

	//COM_ERROR_IF_FAILED(hr, "Failed to initialize vertex buffer for sprite.");

	resource.As(&texture2d);

	CD3D11_TEXTURE2D_DESC texDesc;
	texture2d->GetDesc(&texDesc);

	origin.x = float(texDesc.Width / 2);
	origin.y = float(texDesc.Height / 2);

	return true;
}

float Sprite::GetWidth()
{
	return origin.x;
}

float Sprite::GetHeight()
{
	return origin.y;
}

ID3D11ShaderResourceView* Sprite::Get()
{
	return this->view.Get();
}

ID3D11ShaderResourceView** Sprite::GetTextureResourceViewAddress()
{
	return this->view.GetAddressOf();
}
