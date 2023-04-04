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
	bool Init(const std::string& name, ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string spritePath, SpriteBatch* spriteBatch);
	float GetWidth();
	float GetHeight();
	DirectX::SimpleMath::Vector2 origin;
	/// <summary>
	/// �ؽ��ĸ� �ּҸ� �����ɴϴ�.
	/// ImGui���� ����ϼ���
	/// </summary>
	/// <returns></returns>
	ID3D11ShaderResourceView* Get();
	/// <summary>
	/// �ؽ��ĸ� ����Ű�� ������ ������ �ּҸ� �����ɴϴ�.
	/// </summary>
	/// <returns></returns>
	ID3D11ShaderResourceView** GetTextureResourceViewAddress();
	SpriteBatch* GetSpriteBatch();
	std::string GetName() { return name; }

private:

	std::string name;
	SpriteBatch* spriteBatch;
	ID3D11DeviceContext* deviceContext = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view = nullptr;
	
};

