#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "ConstantBufferType.h"
#include "Texture.h"
#include "GameObject.h"
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <SpriteBatch.h>

class Sprite
{
public:
	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string spritePath, SpriteBatch* spriteBatch);
	float GetWidth();
	float GetHeight();
	DirectX::SimpleMath::Vector2 origin;
	ID3D11ShaderResourceView* Get();
	ID3D11ShaderResourceView** GetTextureResourceViewAddress();
	SpriteBatch* GetSpriteBatch();

private:
	SpriteBatch* spriteBatch;
	ID3D11DeviceContext* deviceContext = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view = nullptr;
	
};

