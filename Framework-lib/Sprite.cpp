#include "pch.h"
#include "Sprite.h"

bool Sprite::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string spritePath,SpriteBatch* spriteBatch)
{
	this->spriteBatch = spriteBatch;
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;

	HRESULT hr = DirectX::CreateWICTextureFromFile(device, StringUtil::StringToWide(spritePath).c_str(),
			resource.GetAddressOf(),
			this->view.GetAddressOf());

	if (FAILED(hr))
	{
		Logger::Log("해당 경로에서 Sprite를 읽어올수 없습니다! 경로: " + spritePath);
		return false;
	}
		
	resource.As(&texture2d);

	CD3D11_TEXTURE2D_DESC texDesc;
	texture2d->GetDesc(&texDesc);

	origin.x = float(texDesc.Width / 2);
	origin.y = float(texDesc.Height / 2);

	return true;
}

float Sprite::GetWidth()
{
	return origin.x * 2;
}

float Sprite::GetHeight()
{
	return origin.y * 2;
}

ID3D11ShaderResourceView* Sprite::Get()
{
	return this->view.Get();
}

ID3D11ShaderResourceView** Sprite::GetTextureResourceViewAddress()
{
	return this->view.GetAddressOf();
}

SpriteBatch* Sprite::GetSpriteBatch()
{
	return this->spriteBatch;
}
