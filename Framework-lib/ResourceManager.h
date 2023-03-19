#pragma once
#include "pch.h"
#include "ConstantBuffer.h"
#include "ConstantBufferType.h"
#include "Sprite.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include "Shaders.h"
class Framework;

class ResourceManager
{
public:
	ResourceManager() {};

	void Init(Framework *framework);
	void InitFont(const std::wstring& path);
	Sprite* LoadSprite(const std::string& name, const std::string& spritePath);
	bool LoadVertexShader(const std::string& name, const std::wstring& path, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements2D);
	bool LoadPixelShader(const std::string& name, const std::wstring& path);
	Sprite* FindSprite(const std::string& name);
	VertexShader* FindVertexShader(const std::string& key);
	PixelShader* FindPixelShader(const std::string& key);

	//스프라이트
	std::unordered_map<std::string, Sprite*> _spriteMap;

	//버텍스 쉐이더
	std::unordered_map<std::string, VertexShader*> _vsMap;
	std::unordered_map<std::string, PixelShader*> _psMap;

	//상수버퍼
	ConstantBuffer<CB_VS_1> cb1;
	ConstantBuffer<CB_VS_2> cb2;
	ConstantBuffer<CB_VS_2> cb_skinning_1;
	ConstantBuffer<CB_VS_3> cb_skinning_2;
	ConstantBuffer<CB_PS_light> cb_light;

	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<SpriteBatch> spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;

	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;

};